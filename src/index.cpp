#include <algorithm>
#include <iostream>
#include <optional>
#include <string>
#include <tuple>
#include <filesystem>

#include <libexif/exif-data.h>
#include <libexif/exif-tag.h>
#include <libexif/exif-utils.h>

#include <spdlog/spdlog.h>

#include <xapian.h>

constexpr auto INDEX_PATH = "./index_data";
constexpr auto F_DOCID = 1;
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

        std::cout << degrees << minutes << seconds << std::endl;
        return degrees + (minutes/60.0) + (seconds/3600.0);
    }

    // not sure how to handle less components yet
    return std::nullopt;
}

static int index_file(Xapian::TermGenerator &indexer, ExifData *exifData)
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
            std::cout << "GPS Lat: " << lat.value() << std::endl;
        } else {
            std::cout << "No GPS Lat: " << std::endl;
        }
        exif_entry_unref(exifEntry);
    }
    return 0;
}

int iu_index_directory_recursive(const std::string &root)
{
    Xapian::WritableDatabase db(std::string(INDEX_PATH), Xapian::DB_CREATE_OR_OPEN);
    Xapian::TermGenerator indexer;
    int failed_count = 0;

    for (auto& p: fs::recursive_directory_iterator(root)) {
        std::string ext(p.path().extension());
        std::transform(ext.begin(), ext.end(), ext.begin(),
                       [](unsigned char c){ return std::tolower(c); });
        if (ext != ".jpg" && ext != ".jpeg") {
            continue;
        }
        spdlog::debug("processing: {}", p.path().string());

        Xapian::Document doc;
        indexer.set_document(doc);
        doc.add_value(F_DOCID, p.path().string());

        ExifData *exifData = exif_data_new_from_file(p.path().c_str());
        if (!exifData) {
            spdlog::error("Failed to load {}", p.path().string());
            failed_count++;
            continue;
        }
        exif_data_fix(exifData);
        exif_data_dump(exifData);

        index_file(indexer, exifData);

        exif_data_unref(exifData);

        std::string idterm("Q" + p.path().string());
        doc.add_boolean_term(idterm);
        db.replace_document(idterm, doc);
    }
    db.commit();
    std::cout << "indexed: " << db.get_doccount() << " files" << std::endl;
    if (failed_count > 0) {
        std::cout << "failed: " << failed_count << " files" << std::endl;
    }
    return 0;
}

