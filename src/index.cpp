#include <xapian.h>

#include <algorithm>
#include <libexif/exif-data.h>
#include <libexif/exif-tag.h>
#include <libexif/exif-utils.h>

#include <iostream>
#include <optional>
#include <string>
#include <tuple>
#include <filesystem>

#include "spdlog/spdlog.h"

#define INDEX_PATH "./index_data"
#define F_DOCID 1
#define BUFFER_SIZE 255

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
    {
        ExifEntry *exifEntry = exif_data_get_entry(exifData, EXIF_TAG_MODEL);
        if (exifEntry != nullptr) {
            char buf[255];
            const char * val = exif_entry_get_value(exifEntry, buf, 255);
            // C: camera
            indexer.index_text(val, 1, "C");
            std::cout << "Model: " << val << std::endl;
        }
        exif_entry_unref(exifEntry);
    }

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
        exif_data_dump(exifData);

        index_file(indexer, exifData);

        exif_data_unref(exifData);

        db.add_document(doc);
    }
    db.commit();
    return 0;
}

