#pragma once

#include <filesystem>
#include <string>

namespace iu {

namespace fs = std::filesystem;

struct index_opts {
    std::string root = fs::current_path();
    bool detect_objects = false;
    bool detect_place_names = true;
};

int index_directory_recursive(const index_opts &opts);

}
