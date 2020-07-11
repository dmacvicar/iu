#include <cmath>

#include <csv2/reader.hpp>
#include <spdlog/spdlog.h>

#include "gps.hpp"

void load_location_countries(std::map<std::string, std::string> &countries) {
    if (countries.empty()) {
        // insert all countries in lookup table
        csv2::Reader<csv2::delimiter<','>,
                     csv2::quote_character<'\"'>,
                     csv2::first_row_is_header<false>,
                     csv2::trim_policy::trim_whitespace> csv;
        if (csv.mmap("countries.csv")) {
            for (const auto row: csv) {
                auto it = row.begin();
                std::string key, name;
                (*it).read_value(key); ++it;
                (*it).read_value(name);
                countries.insert(std::pair<std::string, std::string>(key, name));
            }
        }
        spdlog::debug("Loaded {} countries", countries.size());
    }
}

void load_location_geos(std::vector<std::tuple<double, double, std::string, std::string>> &geo)
{
    if (geo.empty()) {
        // insert all countries in lookup table
        csv2::Reader<csv2::delimiter<','>,
                     csv2::quote_character<'\"'>,
                     csv2::first_row_is_header<false>,
                     csv2::trim_policy::trim_whitespace> csv;
        if (csv.mmap("geocode.csv")) {
            for (const auto row: csv) {
                auto it = row.begin();
                std::string lats, lons, cc, name;
                (*it).read_value(lats); ++it;
                (*it).read_value(lons); ++it;
                (*it).read_value(cc); ++it;
                (*it).read_value(name);
                try {
                    geo.push_back(std::make_tuple(std::stod(lats), std::stod(lons), cc, name));
                } catch (std::invalid_argument e) {
                    spdlog::error("Could not convert {} {} {} {} to numbers", lats, lons, cc, name);
                    continue;
                }
            }
        }
        spdlog::debug("Loaded {} locations", geo.size());
    }
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
    spdlog::debug("Photo taken near {}, {} ({} km)", place, country->second, *min);
    return std::make_tuple(country->second, place);
}

