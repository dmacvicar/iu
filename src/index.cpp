#include <algorithm>
#include <iostream>
#include <optional>
#include <string>
#include <tuple>
#include <filesystem>

#include <spdlog/spdlog.h>

#include <xapian.h>

#include "index.hpp"
#include "detect_objects.hpp"

namespace iu {

constexpr auto INDEX_PATH = "./index_data";
constexpr auto F_DOCID = 1;

namespace fs = std::filesystem;

extern int iu_index_file(Xapian::TermGenerator &indexer, const fs::path &file_path);

int index_directory_recursive(const index_opts &opts)
{
    Xapian::WritableDatabase db(std::string(INDEX_PATH), Xapian::DB_CREATE_OR_OPEN);
    Xapian::TermGenerator indexer;
    int failed_count = 0;

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
        doc.add_value(F_DOCID, p.path().string());

        // call the compile-time backend version
        if (iu_index_file(indexer, p.path()) != 0) {
            spdlog::error("Error while indexing {} metadata", p.path().string());
            continue;
        }

        // object detection
        auto labels = detect_objects(p.path().string());
        for (auto label: labels) {
            indexer.index_text(label, 1, "O");

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

} // namespace iu
