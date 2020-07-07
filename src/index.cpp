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

extern int iu_index_file(Xapian::TermGenerator &indexer, const fs::path &file_path);

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

        // call the compile-time backend version
        if (iu_index_file(indexer, p.path()) != 0) {
            spdlog::error("Error while indexing {} metadata", p.path().string());
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

