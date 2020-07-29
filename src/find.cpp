#include <iostream>
#include <string>

#include <spdlog/spdlog.h>
#include <xapian.h>

#include "index.hpp"
#include "find.hpp"

namespace iu {

constexpr auto INDEX_PATH = "./index_data";

int search(const std::string &query_str, std::function<void(const std::string)>  cb)
{
    try {
        std::string path(INDEX_PATH);
        Xapian::Database db(path);
        Xapian::Enquire enquire(db);
        Xapian::QueryParser qp;

        std::string date_prefix(FIELD_DATE_PREFIX);
        Xapian::DateRangeProcessor date_vrp(FIELD_DATE_NO, "date:");
        qp.add_rangeprocessor(&date_vrp);
        
        std::map<std::string, std::string> fields = {{FIELD_CAMERA_NAME, FIELD_CAMERA_PREFIX},
                                                     {FIELD_DATE_NAME, FIELD_DATE_PREFIX},
                                                     {FIELD_OBJECT_NAME, FIELD_OBJECT_PREFIX},
                                                     {FIELD_PLACE_NAME, FIELD_PLACE_PREFIX},
                                                     {FIELD_FILE_NAME, FIELD_FILE_PREFIX}};
        for (auto& [name, prefix]: fields) {
            spdlog::debug("Adding shortuct '{}' for prefix '{}'", name, prefix);
            qp.add_prefix(name, prefix);
        }

        Xapian::Query query = qp.parse_query(query_str);
        spdlog::debug("Query: {}", query.get_description());

        enquire.set_query(query);
        auto max_items = db.get_doccount();
        Xapian::MSet result = enquire.get_mset(0, max_items);
        spdlog::info("{} results found", result.get_matches_estimated());
        for(Xapian::MSetIterator iter = result.begin(); iter != result.end(); iter++){
            Xapian::Document doc = iter.get_document();
            cb(doc.get_value(FIELD_ID_NO));
        }

    } catch(const Xapian::Error e) {
        std::cerr << e.get_description() << std::endl;
        return -1;
    }
    return 0;
}

} // namespace iu
