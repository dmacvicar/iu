#include <iostream>
#include <string>

#include <spdlog/spdlog.h>
#include <xapian.h>

#include "find.hpp"

constexpr auto INDEX_PATH = "./index_data";
constexpr auto F_DOCID = 1;

int iu_search(const std::string &query_str, std::function<void(const std::string)>  cb)
{
    try {
        std::string path(INDEX_PATH);
        Xapian::Database db(path);
        Xapian::Enquire enquire(db);

        Xapian::QueryParser qp;
        qp.add_prefix("camera", "C");
        qp.add_prefix("file", "Q");
        qp.add_prefix("place", "P");
        Xapian::Query query = qp.parse_query(query_str);
        spdlog::debug("Query: {}", query.get_description());

        //find the top 10 result
        enquire.set_query(query);
        auto max_items = db.get_doccount();
        Xapian::MSet result = enquire.get_mset(0, max_items);
        spdlog::info("{} results found", result.get_matches_estimated());
        for(Xapian::MSetIterator iter = result.begin(); iter != result.end(); iter++){
            Xapian::Document doc = iter.get_document();
            cb(doc.get_value(F_DOCID));
        }

    } catch(const Xapian::Error e) {
        std::cerr << e.get_description() << std::endl;
        return -1;
    }
    return 0;
}
