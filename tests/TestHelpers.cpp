#include <catch_amalgamated.hpp>

#include "util/Helpers.h"

TEST_CASE("can convert string to int")
{
	SECTION("read numbers")
	{
		auto value = Helpers::StringToInt("1024");

		REQUIRE(value == 1024);
	}

	SECTION("read units")
	{
		auto value1 = Helpers::StringToInt("256K");
		auto value2 = Helpers::StringToInt("256M");

		REQUIRE(value1 == 0x40000);
		REQUIRE(value2 == 0x10000000);
	}

	SECTION("returns a default")
	{
		auto value = Helpers::StringToInt("lara", 42);

		REQUIRE(value == 42);
	}
}