#pragma once

#include <filesystem>
#include <map>
#include <string>
#include <optional>
#include <utility>

namespace iu {

namespace fs = std::filesystem;

struct image_metadata {
    std::optional<std::pair<double, double>> location;
    std::set<std::string> camera;
};

constexpr auto FIELD_CAMERA_PREFIX = "C";
constexpr auto FIELD_OBJECT_PREFIX = "O";
constexpr auto FIELD_PLACE_PREFIX = "P";
constexpr auto FIELD_FILE_PREFIX = "Q";

constexpr auto FIELD_CAMERA_NAME = "camera";
constexpr auto FIELD_PLACE_NAME = "place";
constexpr auto FIELD_OBJECT_NAME = "object";
constexpr auto FIELD_FILE_NAME = "file";

struct index_opts {
    std::string root = fs::current_path();
    bool detect_objects = false;
    bool detect_place_names = true;
};

int index_directory_recursive(const index_opts &opts);

} // namespace iu
