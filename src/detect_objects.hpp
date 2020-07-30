#pragma once

#include <filesystem>
#include <set>

namespace iu {

int detect_objects(std::set<std::string> &labels, const std::filesystem::path p);

}
