#pragma once

#include <filesystem>
#include <set>

std::set<std::string> detect_objects(const std::filesystem::path p);
