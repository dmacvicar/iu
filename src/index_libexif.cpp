#include <string>
#include <filesystem>

#include <libexif/exif-data.h>
#include <libexif/exif-tag.h>
#include <libexif/exif-utils.h>

#include <spdlog/spdlog.h>

#include <xapian.h>

constexpr auto BUFFER_SIZE = 255;

namespace fs = std::filesystem;

static std::optional<float> exifentry_to_degrees(ExifEntry *exifEntry)
{
    if (exifEntry == nullptr || exifEntry->format != EXIF_FORMAT_RATIONAL) {
        return std::nullopt;
    }

    ExifByteOrder order = exif_data_get_byte_order(exifEntry->parent->parent);
    // http://www.opanda.com/en/pe/help/gps.html#GPSLatitude
    if (exifEntry->components == 3) {
        // When degrees, minutes and seconds are expressed, the format is dd/1,mm/1,ss/1
        auto degrees = exif_get_rational(exifEntry->data, order).numerator;
        auto minutes = exif_get_rational(exifEntry->data + sizeof(ExifRational), order).numerator;
        auto seconds = exif_get_rational(exifEntry->data + 2*sizeof(ExifRational), order).numerator;

        return degrees + (minutes/60.0) + (seconds/3600.0);
    }

    // not sure how to handle less components yet
    return std::nullopt;
}

static int index_exif_data(Xapian::TermGenerator &indexer, ExifData *exifData)
{
    static char buf[BUFFER_SIZE];
    {
        for (auto tag: {EXIF_TAG_MAKE, EXIF_TAG_MODEL}) {
            ExifEntry *exifEntry = exif_data_get_entry(exifData, tag);
            if (exifEntry == nullptr) {
                continue;
            }
            const char * val = exif_entry_get_value(exifEntry, buf, BUFFER_SIZE);
            // C: camera
            indexer.index_text(val, 1, "C");
            exif_entry_unref(exifEntry);
        }
    }
    return 0;
    {
        ExifEntry *exifEntry = exif_data_get_entry(exifData, (ExifTag) EXIF_TAG_GPS_LATITUDE);
        auto lat = exifentry_to_degrees(exifEntry);
        if (lat) {
            spdlog::debug("GPS Lat: {}", lat.value());
        } else {
            spdlog::debug("No GPS Lat");
        }
        exif_entry_unref(exifEntry);
    }
    return 0;
}

int iu_index_file(Xapian::TermGenerator &indexer, const fs::path &p)
{
    ExifData *exifData = exif_data_new_from_file(p.c_str());
    if (!exifData) {
        spdlog::error("Failed to load {}", p.string());
        return -1;
    }
    exif_data_fix(exifData);
    //exif_data_dump(exifData);

    auto ret = index_exif_data(indexer, exifData);
    if (ret != 0) {
        spdlog::error("Failed to index {}", p.string());
        return -1;
    }

    // FIXME, still crashes on a bunch of photos
    //exif_data_unref(exifData);
    return 0;
}

