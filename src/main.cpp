#include <iostream>
#include <string>
#include <vector>

#include <CLI/CLI.hpp>
#include <spdlog/spdlog.h>

#include "find.hpp"
#include "index.hpp"

int main(int argc, char* argv[])
{
    CLI::App app("iu - The image indexer");
    app.set_help_all_flag("--help-all", "Expand all help");
    auto *debug = app.add_flag("-d,--debug", "Debug mode");

    CLI::App *index = app.add_subcommand("index", "Index images");
    CLI::App *find = app.add_subcommand("find", "Query the image database");
    app.require_subcommand();

    std::string root;
    // TODO remove required once there is a good default
    index->add_option("-r,--root", root, "Folder root")
        ->required()
        ->check(CLI::ExistingDirectory);

    std::string query;
    find->add_option("query", query, "Query")->required();

    CLI11_PARSE(app, argc, argv);

    if (debug->count() > 0) {
        spdlog::set_level(spdlog::level::debug);
    }
    spdlog::debug("debug logging enabled...");

    for (const auto subcom : app.get_subcommands()) {
        if (subcom == index) {
            iu_index_directory_recursive(root);
        } else if (subcom == find) {
            if (iu_search(query) != 0) {
                return -1;
            }
        }
    }

    return 0;
}
