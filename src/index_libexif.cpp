#include <cstdlib>
#include <string>
#include <filesystem>
#include <map>
#include <vector>

#include <libexif/exif-data.h>
#include <libexif/exif-tag.h>
#include <libexif/exif-utils.h>

#include <csv2/reader.hpp>
#include <spdlog/spdlog.h>

#include <xapian.h>

#include "gps.hpp"

constexpr auto BUFFER_SIZE = 2000;

namespace fs = std::filesystem;

static double exif_value_to_degrees(const char *s)
{
    char *pos;
    double degrees = strtod(s, &pos);
    degrees += strtod(pos + 1, &pos) / 60.0;
    degrees += strtod(pos + 1, nullptr) / 3600.0;
    return degrees;
}

template <ExifTag tag, ExifTag ref, char pos, char neg>
static std::optional<double> exif_data_lat_lon(ExifData *d)
{
    char buf[2000];

    ExifEntry *e = exif_data_get_entry(d, tag);
    if (!e) {
        return std::nullopt;
    }
    const char *s = exif_entry_get_value(e, buf, sizeof(buf));
    if (!s) {
        return std::nullopt;
    }
    double deg = exif_value_to_degrees(s);

    e = exif_data_get_entry(d, ref);
    if (!e) {
        return std::nullopt;
    }
    s = exif_entry_get_value(e, buf, sizeof(buf));
    if (!s) {
        return std::nullopt;
    }
    if (s[0] == neg) {
        deg = -1 * deg;
    }
    return deg;
}

// returns the location as a pair of signed double
static std::optional<std::tuple<double, double>> location(ExifData *d)
{
    auto lat = exif_data_lat_lon<static_cast<ExifTag>(EXIF_TAG_GPS_LATITUDE), static_cast<ExifTag>(EXIF_TAG_GPS_LATITUDE_REF), 'N', 'S'>(d);
    if (!lat) {
        return std::nullopt;
    }

    auto lon = exif_data_lat_lon<static_cast<ExifTag>(EXIF_TAG_GPS_LONGITUDE), static_cast<ExifTag>(EXIF_TAG_GPS_LONGITUDE_REF), 'E', 'W'>(d);
    if (!lon) {
        return std::nullopt;
    }

    return std::make_optional(std::make_tuple(lat.value(), lon.value()));
}

static int index_exif_data(Xapian::TermGenerator &indexer, ExifData *d)
{
    static char buf[BUFFER_SIZE];
    {
        for (auto tag: {EXIF_TAG_MAKE, EXIF_TAG_MODEL}) {
            ExifEntry *e = exif_data_get_entry(d, tag);
            if (!e) {
                spdlog::debug("Can't get exif entry for tag {}", exif_tag_get_name(tag));;
                continue;
            }
            const char *s = exif_entry_get_value(e, buf, sizeof(buf));
            if (!s) {
                spdlog::debug("Can't get value from entry for tag {}", exif_tag_get_name(tag));
                continue;
            }

            // C: camera
            indexer.index_text(s, 1, "C");
        }
    }

    {
        auto maybe_loc = location(d);
        if (maybe_loc) {
            spdlog::debug("GPS {} {}", std::get<0>(maybe_loc.value()), std::get<1>(maybe_loc.value()));
            auto maybe_text = location_text(std::get<0>(maybe_loc.value()), std::get<1>(maybe_loc.value()));
            if (maybe_text) {
                // country
                indexer.index_text(std::get<0>(maybe_text.value()), 1, "P");
                // place
                indexer.index_text(std::get<1>(maybe_text.value()), 1, "P");
            }
        } else {
            spdlog::debug("No GPS data"); 
        }
    }
    return 0;
}

int iu_index_file(Xapian::TermGenerator &indexer, const fs::path &p)
{
    ExifData *d = exif_data_new_from_file(p.c_str());
    if (!d) {
        spdlog::error("Failed to load {}", p.string());
        return -1;
    }
    exif_data_fix(d);
    //exif_data_dump(d);

    auto ret = index_exif_data(indexer, d);
    if (ret != 0) {
        spdlog::error("Failed to index {}", p.string());
        exif_data_unref(d);
        return -1;
    }

    exif_data_unref(d);
    return 0;
}

