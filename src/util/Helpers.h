#pragma once

#include <string>

class Helpers
{
public:
	// Converts a value to integer with support for units such as megabytes
	static int StringToInt(const std::string& value, int defaultValue);
};