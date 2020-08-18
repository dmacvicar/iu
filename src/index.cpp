#include <algorithm>
#include <iostream>
#include <optional>
#include <string>
#include <tuple>
#include <filesystem>
#include <vector>
#include <utility>
#include <string>

#include <spdlog/spdlog.h>

#include <xapian.h>

#include "index.hpp"
#include "gps.hpp"
#include "detect_objects.hpp"

namespace iu {

constexpr auto INDEX_PATH = "./index_data";

namespace fs = std::filesystem;

extern int file_gather_metadata(const index_opts &opts, image_metadata &md, const fs::path &file_path);

void index_directory_recursive(const index_opts &opts)
{
    Xapian::WritableDatabase db(std::string(INDEX_PATH), Xapian::DB_CREATE_OR_OPEN);
    Xapian::TermGenerator indexer;
    int failed_count = 0;

    auto camera_prefix(FIELD_CAMERA_PREFIX);
    auto date_prefix(FIELD_DATE_PREFIX);
    auto file_prefix(FIELD_FILE_PREFIX);
    auto object_prefix(FIELD_OBJECT_PREFIX);
    auto place_prefix(FIELD_PLACE_PREFIX);

    for (auto& p: fs::recursive_directory_iterator(opts.root)) {
        std::string ext(p.path().extension());
        std::transform(ext.begin(), ext.end(), ext.begin(),
                       [](unsigned char c){ return std::tolower(c); });
        if (ext != ".jpg" && ext != ".jpeg") {
            continue;
        }
        spdlog::debug("processing: {}", p.path().string());

        Xapian::Document doc;
        indexer.set_document(doc);
        doc.add_value(FIELD_ID_NO, p.path().string());

        image_metadata md;
        // call the compile-time backend version
        if (file_gather_metadata(opts, md, p.path()) != 0) {
            // we only log this error, but still move forward with other metadata
            spdlog::error("Error while gathering metadata: {}", p.path().string());
        }

        // camera
        for (auto term: md.camera) {
            indexer.index_text(term, 1, camera_prefix);
        }

        // places
        if (opts.detect_place_names && md.location) {
            spdlog::debug("Location {:.2f}, {:.2f}", std::get<0>(md.location.value()), std::get<1>(md.location.value()));
            auto lat = std::get<0>(md.location.value());
            auto lon = std::get<1>(md.location.value());
            auto maybe_text = location_text(lat, lon);
            if (maybe_text) {
                auto country = std::get<0>(maybe_text.value());
                auto place = std::get<1>(maybe_text.value());

                indexer.index_text(country, 1, place_prefix);
                indexer.index_text(place, 1, place_prefix);
            }
        } else {
            spdlog::debug("No GPS data"); 
        }

        // object detection
        if (opts.detect_objects) {
            std::set<std::string> labels;
            try {
                detect_objects(labels, p.path().string());
            } catch(...) {
                std::throw_with_nested(std::runtime_error("Failed to execute image classifier"));
            }

            for (auto label: labels) {
                indexer.index_text(label, 1, object_prefix);
            }
        }

        // date
        if (md.date_time) {
            std::array<char, 2000> buf;
            auto len = std::strftime(buf.data(), buf.size(), "%Y%m%d", &md.date_time.value());
            auto date = std::string(buf.data(), len);
            spdlog::debug("date: {}", date);
            doc.add_value(FIELD_DATE_NO, date);
        }

        std::string idterm(file_prefix + p.path().string());
        doc.add_boolean_term(idterm);
        db.replace_document(idterm, doc);
    }
    db.commit();

    std::cout << "indexed: " << db.get_doccount() << " files" << std::endl;
    if (failed_count > 0) {
        std::cout << "failed: " << failed_count << " files" << std::endl;
    }
}

} // namespace iu
