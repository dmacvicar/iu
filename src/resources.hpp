#pragma once

#include <optional>
#include <string>
#include <filesystem>

namespace iu {

std::optional<std::filesystem::path> find_resource(const std::string &name);

std::filesystem::path database_path();

}

