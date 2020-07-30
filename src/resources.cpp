#include "resources.hpp"

namespace iu {

namespace fs = std::filesystem;

std::optional<fs::path> find_resource(const std::string &name) {
    auto right_here = fs::current_path() / name;
    if (fs::exists(right_here)) {
        return right_here;
    }
    // TODO later also lookup in system data dir
    return std::nullopt;
}

}
