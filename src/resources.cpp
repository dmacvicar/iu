#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>

#include "resources.hpp"

namespace iu {

namespace fs = std::filesystem;

static std::optional<fs::path> home_path() {
    fs::path home;

    struct passwd* pwd = getpwuid(getuid());
    if (pwd && pwd->pw_dir) {
        home = pwd->pw_dir;
        if (fs::is_directory(home)) {
            return home;
        }
    }

    home = getenv("HOME");
    if (fs::is_directory(home)) {
        return home;
    }
    return std::nullopt;
}

static std::optional<fs::path> local_path() {
    auto home = home_path();
    if (!home) {
        return std::nullopt;
    }
    return home.value() / fs::path(".local/iu");
}

std::filesystem::path database_path() {
    auto local = local_path().value_or(fs::current_path() / fs::path(".iu"));
    return local / fs::path("db");
}

std::optional<fs::path> find_resource(const std::string &name) {
    auto right_here = fs::current_path() / name;
    if (fs::exists(right_here)) {
        return right_here;
    }
    // TODO later also lookup in system data dir
    return std::nullopt;
}


}
