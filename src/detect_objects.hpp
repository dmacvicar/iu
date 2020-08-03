#pragma once

#include <filesystem>
#include <set>

namespace iu {

void detect_objects(std::set<std::string> &labels, const std::filesystem::path p);

}
