#include <xapian.h>

#include <algorithm>
#include <libexif/exif-data.h>
#include <libexif/exif-tag.h>

#include <iostream>
#include <string>
#include <filesystem>

#define ROOT_DIR "/home/duncan/Fake-Photo-Lib-2"
#define INDEX_PATH "./index_data"
#define F_DOCID 1

namespace fs = std::filesystem;

int process_file(Xapian::TermGenerator &indexer, ExifData *exifData) {
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
        if (exifEntry != nullptr) {
            if (exifEntry->format == EXIF_FORMAT_RATIONAL) {
                // TODO parse GPS coordinates
            }
            std::cout << exifEntry->size << std::endl;
        }
        exif_entry_unref(exifEntry);
    }
    return 0;
}

int main(int argc, char *argv[]) {
    Xapian::WritableDatabase db(std::string(INDEX_PATH), Xapian::DB_CREATE_OR_OPEN);
    Xapian::TermGenerator indexer;

    for (auto& p: fs::recursive_directory_iterator(ROOT_DIR)) {
        std::string ext(p.path().extension());
        std::transform(ext.begin(), ext.end(), ext.begin(),
                       [](unsigned char c){ return std::tolower(c); });
        if (ext != ".jpg" && ext != ".jpeg") {
            continue;
        }
        std::cout << p.path() << '\n';
        Xapian::Document doc;
        indexer.set_document(doc);
        doc.add_value(F_DOCID, p.path().string());

        ExifData *exifData = exif_data_new_from_file(p.path().c_str());
        exif_data_dump(exifData);

        process_file(indexer, exifData);

        exif_data_free(exifData);

        db.add_document(doc);
    }
    db.commit();
    return 0;
}

