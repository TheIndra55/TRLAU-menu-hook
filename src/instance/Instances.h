#pragma once

#include <functional>

#include "Instance.h"

class Instances
{
public:
	static void Iterate(std::function<void(Instance*)> callback) noexcept;
};