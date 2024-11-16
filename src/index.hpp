#pragma once

#include <ctime>
#include <filesystem>
#include <optional>
#include <set>
#include <string>
#include <utility>

namespace iu {

struct image_metadata {
    std::optional<std::pair<double, double>> location;
    std::set<std::string> camera;
    std::optional<std::tm> date_time;
    std::optional<long> width;
    std::optional<long> height;
};

constexpr auto FIELD_CAMERA_PREFIX = "C";
constexpr auto FIELD_ENTITY_PREFIX = "XE";
constexpr auto FIELD_AI_PREFIX = "XZ";

constexpr auto FIELD_PLACE_PREFIX = "XP";
constexpr auto FIELD_FILE_PREFIX = "Q";
constexpr auto FIELD_DATE_PREFIX = "D";
constexpr auto FIELD_QUALITY_PREFIX = "XQ";
constexpr auto FIELD_LOCATION_PREFIX = "XL";

constexpr auto FIELD_CAMERA_NAME = "camera";
constexpr auto FIELD_PLACE_NAME = "place";
constexpr auto FIELD_ENTITY_NAME = "entity";
constexpr auto FIELD_AI_NAME = "ai";

constexpr auto FIELD_FILE_NAME = "file";
constexpr auto FIELD_DATE_NAME = "date";
constexpr auto FIELD_QUALITY_NAME = "quality";
constexpr auto FIELD_LOCATION_NAME = "location";

constexpr auto FIELD_ID_NO = 1;
constexpr auto FIELD_DATE_NO = 2;
constexpr auto FIELD_QUALITY_NO = 3;
constexpr auto FIELD_LOCATION_NO = 4;

struct index_opts {
    std::string root = std::filesystem::current_path();
    bool index_entity_names = false;

    bool index_ai = false;
    std::string ai_base_url;
    std::string ai_api_key;
    std::string ai_model;

    bool index_location_names = true;
};

void index_directory_recursive(const index_opts &opts);

} // namespace iu
