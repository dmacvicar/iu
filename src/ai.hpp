#pragma once

#include <filesystem>
#include <set>

#include "index.hpp"

namespace iu {

void ai_init(const index_opts &opts);
void ai_describe_image(const index_opts &opts, std::set<std::string> &labels, const std::filesystem::path p);

}
