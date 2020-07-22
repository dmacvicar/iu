#pragma once
#include <functional>

namespace iu {

int search(const std::string &query_str, std::function<void(const std::string)>  cb);

}
