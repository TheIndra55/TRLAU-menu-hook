#include "Helpers.h"

#include <charconv>
#include <string>
#include <sstream>

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

std::string Helpers::ToHex(size_t value)
{
	std::stringstream stream;
	stream << std::hex << value;
	return stream.str();
}