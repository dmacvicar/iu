#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "location.hpp"

using Catch::WithinAbs;

TEST_CASE("Haversine formula")
{
    // Big Ben in London (51.5007° N, 0.1246° W)
    // The Statue of Liberty in New York (40.6892° N, 74.0445° W) is 5574.8 km.
    auto d = iu::haversine_distance(51.5007, -0.1246, 40.6892, -74.0445);
    REQUIRE_THAT(d, WithinAbs(5574.840, 0.001));
}
