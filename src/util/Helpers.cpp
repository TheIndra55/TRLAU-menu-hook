#include "Helpers.h"

#include <charconv>

int Helpers::StringToInt(const std::string& str, int defaultValue)
{
	int value;
	auto [p, ec] = std::from_chars(str.data(), str.data() + str.size(), value);

	if (ec != std::errc())
	{
		return defaultValue;
	}

	if (p[0] == 'M')
	{
		value <<= 20;
	}

	if (p[0] == 'K')
	{
		value <<= 10;
	}

	return value;
}