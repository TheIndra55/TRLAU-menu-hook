#include <catch_amalgamated.hpp>
#include <math.h>

#include "cdc/math/Vector.h"

TEST_CASE("vector3")
{
	SECTION("can calculate distance")
	{
		auto first  = cdc::Vector3{ 100.f, 11.f, 300.f };
		auto second = cdc::Vector3{ 400.f, 10.f, 230.f };
		auto distance = first - &second;

		REQUIRE(floor(distance) == 308);
	}
}