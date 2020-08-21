#pragma once

#include <map>
#include <string>
#include <vector>
#include <tuple>
#include <optional>

namespace iu {

void load_location_countries(std::map<std::string, std::string> &countries);
void load_location_geos(std::vector<std::tuple<double, double, std::string, std::string>> &geo);
double haversine_distance(double lat1, double lon1, double lat2, double lon2);
std::optional<std::tuple<std::string, std::string>> location_text(double lat, double lon);
void all_locations(std::function<void(const std::string)> cb);

} // namespace ns
