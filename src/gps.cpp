#include <cmath>
#include <filesystem>
#include <optional>

#include <csv2/reader.hpp>
#include <spdlog/spdlog.h>

#include "gps.hpp"
#include "resources.hpp"

namespace iu {

namespace fs = std::filesystem;

void load_location_countries(std::map<std::string, std::string> &countries) {
    if (!countries.empty()) {
        return;
    }

    auto countries_csv = find_resource("countries.csv");
    if (!countries_csv) {
        throw std::runtime_error("Can't find country data, used for geolocation");
    }

    // insert all countries in lookup table
    csv2::Reader<csv2::delimiter<','>,
                 csv2::quote_character<'\"'>,
                 csv2::first_row_is_header<false>,
                 csv2::trim_policy::trim_whitespace> csv;
    if (csv.mmap(countries_csv->string())) {
        int line = 0;
        for (const auto row: csv) {
            line++;
            auto it = row.begin();
            std::string key, name;
            (*it).read_value(key); ++it;
            (*it).read_value(name);
            if (key.empty() || name.empty()) {
                spdlog::warn("Coult not parse '{}': line {}", countries_csv->string(), line);
                break;
            }
            countries.insert(std::pair<std::string, std::string>(key, name));
        }
    }
    spdlog::debug("Loaded {} countries", countries.size());
}

void load_location_geos(std::vector<std::tuple<double, double, std::string, std::string>> &geo)
{
    if (!geo.empty()) {
        return;
    }

    auto geocode_csv = find_resource("geocode.csv");
    if (!geocode_csv) {
        throw std::runtime_error("Can't find geocode data, used for geolocation");
    }

    // insert all countries in lookup table
    csv2::Reader<csv2::delimiter<','>,
                 csv2::quote_character<'\"'>,
                 csv2::first_row_is_header<false>,
                 csv2::trim_policy::no_trimming> csv;
    if (csv.mmap(geocode_csv->string())) {
        int line = 0;
        for (const auto row: csv) {
            line++;
            auto it = row.begin();
            std::string lats, lons, cc, name;
            (*it).read_value(lats); ++it;
            (*it).read_value(lons); ++it;
            (*it).read_value(cc); ++it;
            (*it).read_value(name); ++it;
            try {
                geo.push_back(std::make_tuple(std::stod(lats), std::stod(lons), cc, name));
            } catch (std::invalid_argument e) {
                spdlog::warn("Coult not parse '{}': line {}", geocode_csv->string(), line);
                continue;
            }
        }
    } else {
        throw std::runtime_error(fmt::format("Unable to parse geocode data: '{}'", (*geocode_csv).string()));
    }
    spdlog::debug("Loaded {} locations", geo.size());
}

// https://en.wikipedia.org/wiki/Haversine_formula
double haversine_distance(double lat1, double lon1, double lat2, double lon2)
{
    static constexpr auto r = 6371;

    // convert to radians
    auto lat1r = lat1 * M_PI / 180;
    auto lon1r = lon1 * M_PI / 180;
    auto lat2r = lat2 * M_PI / 180;
    auto lon2r = lon2 * M_PI / 180;

    auto d = 2 * r * std::asin(std::sqrt(std::pow(std::sin((lat1r - lat2r)/2), 2) + std::cos(lat1r) * std::cos(lat2r) * std::pow(std::sin((lon1r - lon2r)/2), 2)));
    return d;
}

// return city/place, country
std::optional<std::tuple<std::string, std::string>> location_text(double lat, double lon) {
    static std::map<std::string, std::string> countries;
    static std::vector<std::tuple<double, double, std::string, std::string>> geo;

    load_location_countries(countries);
    load_location_geos(geo);

    // find the minimum distance
    std::optional<double> min;
    std::optional<std::tuple<double, double, std::string, std::string>> closest_entry;

    for (const auto &entry: geo) {
        auto lat2 = std::get<0>(entry);
        auto lon2 = std::get<1>(entry);

        auto d = haversine_distance(lat, lon, lat2, lon2);
        if (!min || d < *min) {
            min = d;
            closest_entry = entry;
        }
    }

    if (!closest_entry) {
        return std::nullopt;
    }

    auto country_code = std::get<2>(*closest_entry);
    auto country = countries.find(country_code);
    if (country == countries.end()) {
        return std::nullopt;
    }
    auto place = std::get<3>(*closest_entry);
    spdlog::debug("Photo taken near {}, {} ({:.2f} km)", place, country->second, *min);
    return std::make_tuple(country->second, place);
}

} // namespace iu
