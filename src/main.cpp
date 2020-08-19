#include <iostream>
#include <string>
#include <vector>
#include <numeric>

#include <CLI/CLI.hpp>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "find.hpp"
#include "index.hpp"
#include "location.hpp"

void print_exception(const std::exception& e, int level =  0)
{
    spdlog::error("{}{}{}", std::string(level*2, ' '), level == 0 ? "" : "`-> ", e.what());
    try {
        std::rethrow_if_nested(e);
    } catch(const std::exception& e) {
        print_exception(e, level+1);
    } catch(...) {
        spdlog::error("oh no");
    }
}

int main(int argc, char* argv[])
{
    auto logger = spdlog::stderr_color_mt("stderr");
    spdlog::set_default_logger(logger);
    logger->set_pattern("[%^%l%$] %v");

    CLI::App app("iu - The image indexer");
    app.set_help_all_flag("--help-all", "Expand all help");
    auto *debug = app.add_flag("-d,--debug", "Debug mode");

    CLI::App *index = app.add_subcommand("index", "Index images");
    CLI::App *find = app.add_subcommand("find", "Query the image database");
    CLI::App *locations = app.add_subcommand("locations", "Show all locations");
    app.require_subcommand();

    iu::index_opts index_opts;
    // TODO remove required once there is a good default
    index->add_option("-r,--root", index_opts.root, "Folder root")
        ->required()
        ->check(CLI::ExistingDirectory);

    index->add_flag("-e,--index-entity-names,!--skip-index-entity-names", index_opts.index_entity_names, "Index entities despicted in pictures by name");
    index->add_flag("-d,--index-location-names-,!--skip-index-location-names", index_opts.index_location_names, "Index locations by their name");

    std::string query;
    find->add_option("query", query, "Query")->required();
    auto *browse = find->add_flag("-b,--browse", "Browse results");

    CLI11_PARSE(app, argc, argv);

    if (debug->count() > 0) {
        spdlog::set_level(spdlog::level::debug);
    }
    spdlog::debug("debug logging enabled...");

    try {
        for (auto *const subcom : app.get_subcommands()) {
            if (subcom == index) {
                iu::index_directory_recursive(index_opts);
            } else if (subcom == find) {
                if (browse->count() > 0) {
                    std::stringstream cmd;
                    cmd << "eog ";
                    iu::search(query, [&cmd](const std::string &result) { 
                                          cmd << "\"" <<  result << "\" ";
                                                 });
                    std::cout << cmd.str().c_str() << std::endl;
                    system(cmd.str().c_str());
                } else {
                    iu::search(query, [](const std::string &result) {
                                          std::cout << result << std::endl;
                                      });
                }
            } else if (subcom == locations) {
                
            }
        }
    }
    catch (const std::exception &e) {
        print_exception(e);
        return -1;
    }
    return 0;
}
