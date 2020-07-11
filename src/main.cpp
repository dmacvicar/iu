#include <iostream>
#include <string>
#include <vector>
#include <numeric>

#include <CLI/CLI.hpp>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "find.hpp"
#include "index.hpp"

int main(int argc, char* argv[])
{
    auto logger = spdlog::stderr_color_mt("stderr");
    spdlog::set_default_logger(logger);

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
    auto *browse = find->add_flag("-b,--browse", "Browse results");

    CLI11_PARSE(app, argc, argv);

    if (debug->count() > 0) {
        spdlog::set_level(spdlog::level::debug);
    }
    spdlog::debug("debug logging enabled...");

    for (const auto subcom : app.get_subcommands()) {
        if (subcom == index) {
            iu_index_directory_recursive(root);
        } else if (subcom == find) {
            if (browse->count() > 0) {
                std::stringstream cmd;
                cmd << "gthumb ";
                auto ret = iu_search(query, [&cmd](const std::string &result) { 
                                                cmd << "\"" <<  result << "\" ";
                                            });
                if (ret != 0) {
                    return -1;
                }
                //std::system(cmd.str().c_str());
                std::cout << cmd.str().c_str() << std::endl;
            } else {
                auto ret = iu_search(query, [](const std::string &result) {
                                                std::cout << result << std::endl;
                                            });
                if (ret != 0) {
                    return -1;
                }
            }
        }
    }

    return 0;
}