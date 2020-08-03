#pragma once
#include <functional>

namespace iu {

void search(const std::string &query_str, std::function<void(const std::string)> cb);

}
