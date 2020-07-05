#include <xapian.h>
#include <iostream>
#include <string>

#define QUERY "camera:F5321"
#define INDEX_PATH "./index_data"
#define F_DOCID 1

int main() {
  try {
      //The string for query
      std::string query_str(QUERY);

      Xapian::Database db(std::string(INDEX_PATH));
      Xapian::Enquire enquire(db);

      Xapian::QueryParser qp;
      qp.add_prefix("camera", "C");
      Xapian::Query query = qp.parse_query(query_str);
      std::cout << "Query is " << query.get_description() << std::endl;

      //find the top 10 result
      enquire.set_query(query);
      Xapian::MSet result = enquire.get_mset(0, 10);
      std::cout << result.get_matches_estimated() << " result found" << std::endl;

      for(Xapian::MSetIterator iter = result.begin(); iter != result.end(); iter++){
          Xapian::Document doc = iter.get_document();
          std::cout << iter.get_rank() << ": docid " << doc.get_value(F_DOCID) << std::endl;
      }

  } catch(const Xapian::Error e) {
      std::cout << e.get_description() << std::endl;
  }
  return 0;
}
