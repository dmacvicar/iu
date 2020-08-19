#pragma once

#include <filesystem>
#include <map>
#include <string>
#include <optional>
#include <ctime>
#include <utility>

namespace iu {

namespace fs = std::filesystem;

struct image_metadata {
    std::optional<std::pair<double, double>> location;
    std::set<std::string> camera;
    std::optional<std::tm> date_time;
};

constexpr auto FIELD_CAMERA_PREFIX = "C";
constexpr auto FIELD_ENTITY_PREFIX = "XE";
constexpr auto FIELD_PLACE_PREFIX = "P";
constexpr auto FIELD_FILE_PREFIX = "Q";
constexpr auto FIELD_DATE_PREFIX = "D";
constexpr auto FIELD_QUALITY_PREFIX = "XQ";
constexpr auto FIELD_LOCATION_PREFIX = "XL";

constexpr auto FIELD_CAMERA_NAME = "camera";
constexpr auto FIELD_PLACE_NAME = "place";
constexpr auto FIELD_ENTITY_NAME = "entity";
constexpr auto FIELD_FILE_NAME = "file";
constexpr auto FIELD_DATE_NAME = "date";
constexpr auto FIELD_QUALITY_NAME = "quality";
constexpr auto FIELD_LOCATION_NAME = "location";

constexpr auto FIELD_ID_NO = 1;
constexpr auto FIELD_DATE_NO = 2;
constexpr auto FIELD_QUALITY_NO = 3;
constexpr auto FIELD_LOCATION_NO = 4;

struct index_opts {
    std::string root = fs::current_path();
    bool index_entity_names = false;
    bool index_location_names = true;
};

void index_directory_recursive(const index_opts &opts);

} // namespace iu
