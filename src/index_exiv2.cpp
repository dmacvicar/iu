#include <filesystem>

#include <exiv2/exiv2.hpp>
#include <spdlog/spdlog.h>
#include <xapian.h>

namespace fs = std::filesystem;

static int index_exif_data(Xapian::TermGenerator &indexer, Exiv2::ExifData &exifData)
{
    Exiv2::ExifData::const_iterator pos = Exiv2::make(exifData);
    if (pos != exifData.end()) {
        indexer.index_text(pos->toString(), 1, "C");
    }
    pos = Exiv2::model(exifData);
    if (pos != exifData.end()) {
        indexer.index_text(pos->toString(), 1, "C");
    }
    return 0;
}

int iu_index_file(Xapian::TermGenerator &indexer, const fs::path &p)
{
    try {
        auto image = Exiv2::ImageFactory::open(p.string());
        if (image.get() == nullptr) {
            spdlog::error("Failed to load {}", p.string());
            return -1;
        }

        image->readMetadata();
        Exiv2::ExifData &exifData = image->exifData();
        if (exifData.empty()) {
            spdlog::error("No EXIF metadata for {}", p.string());
        }

        index_exif_data(indexer, exifData);

    } catch (Exiv2::Error &e) {
        spdlog::error(e.what());
        return -1;
    }
    return 0;
}
