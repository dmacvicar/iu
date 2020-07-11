#pragma once
#include <functional>

int iu_search(const std::string &query_str, std::function<void(const std::string)>  cb);
