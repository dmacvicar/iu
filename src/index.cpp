#include <algorithm>
#include <iostream>
#include <optional>
#include <string>
#include <tuple>
#include <filesystem>

#include <exiv2/exiv2.hpp>

#include <spdlog/spdlog.h>

#include <xapian.h>

constexpr auto INDEX_PATH = "./index_data";
constexpr auto F_DOCID = 1;

namespace fs = std::filesystem;

static int index_file(Xapian::TermGenerator &indexer, const Exiv2::ExifData &exifData)
{
    static Exiv2::ExifKey EXIF_TAG_MAKE("Exif.Image.Make");
    static Exiv2::ExifKey EXIF_TAG_MODEL("Exif.Image.Model");
    for (auto key: {EXIF_TAG_MAKE, EXIF_TAG_MODEL}) {
        Exiv2::ExifData::const_iterator pos = exifData.findKey(key);
        if (pos != exifData.end()) {
            indexer.index_text(pos->toString(), 1, "C");
        }
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

        try {
            auto image = Exiv2::ImageFactory::open(p.path().string());
            if (image.get() == nullptr) {
                spdlog::error("Failed to load {}", p.path().string());
            }

            image->readMetadata();
            Exiv2::ExifData &exifData = image->exifData();
            if (exifData.empty()) {
                spdlog::error("No EXIF metadata for {}", p.path().string());
            }
            index_file(indexer, exifData);
        } catch (Exiv2::Error &e) {
            spdlog::error(e.what());
            continue;
        }

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

