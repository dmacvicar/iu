#pragma once

#include <functional>
#include <string>

namespace iu {

void search(const std::string &query_str, std::function<void(const std::string)> cb);

}
