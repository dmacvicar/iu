#pragma once

#include <filesystem>
#include <set>

namespace iu {

void detect_entities(std::set<std::string> &labels, const std::filesystem::path p);

}
