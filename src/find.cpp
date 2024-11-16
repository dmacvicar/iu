#include <iostream>
#include <string>

#include <spdlog/spdlog.h>
#include <xapian.h>

#include "index.hpp"
#include "find.hpp"
#include "resources.hpp"

namespace iu {

void search(const std::string &query_str, std::function<void(const std::string)>  cb)
{
    try {
        auto db_path = database_path();
        std::filesystem::create_directories(db_path);
        Xapian::Database db(db_path.string());
        spdlog::debug("Database path: {}", db_path.string());

        Xapian::Enquire enquire(db);
        Xapian::QueryParser qp;

        std::string date_prefix(FIELD_DATE_PREFIX);
        Xapian::DateRangeProcessor date_vrp(FIELD_DATE_NO, "date:");
        qp.add_rangeprocessor(&date_vrp);

        Xapian::NumberRangeProcessor quality_nrp(FIELD_QUALITY_NO, "quality:");
        qp.add_rangeprocessor(&quality_nrp);

        std::map<std::string, std::string> fields = {{FIELD_CAMERA_NAME, FIELD_CAMERA_PREFIX},
                                                     {FIELD_DATE_NAME, FIELD_DATE_PREFIX},
                                                     {FIELD_ENTITY_NAME, FIELD_ENTITY_PREFIX},
                                                     {FIELD_AI_NAME, FIELD_AI_PREFIX},
                                                     {FIELD_PLACE_NAME, FIELD_PLACE_PREFIX},
                                                     {FIELD_FILE_NAME, FIELD_FILE_PREFIX}};
        for (auto& [name, prefix]: fields) {
            spdlog::debug("Adding shortuct '{}' for prefix '{}'", name, prefix);
            qp.add_prefix(name, prefix);
        }
        // include all these when specifying no field
        qp.add_prefix("", FIELD_PLACE_PREFIX);
        qp.add_prefix("", FIELD_FILE_PREFIX);
        qp.add_prefix("", FIELD_AI_PREFIX);
        qp.add_prefix("", FIELD_ENTITY_PREFIX);

        Xapian::Query query = qp.parse_query(query_str);
        if (query_str.empty()) {
            query = Xapian::Query::MatchAll;
        }
        spdlog::debug("Query: {}", query.get_description());

        enquire.set_query(query);
        auto max_items = db.get_doccount();
        Xapian::MSet result = enquire.get_mset(0, max_items);
        spdlog::info("{} results found", result.get_matches_estimated());
        for(Xapian::MSetIterator iter = result.begin(); iter != result.end(); iter++){
            Xapian::Document doc = iter.get_document();
            cb(doc.get_value(FIELD_ID_NO));
        }

    } catch (const Xapian::Error &e) {
        throw std::runtime_error(fmt::format("Search failed. Xapian: {}", e.get_msg()));
    }
}

} // namespace iu
