#pragma once

#include <filesystem>
#include <set>

namespace iu {

std::set<std::string> detect_objects(const std::filesystem::path p);

}
