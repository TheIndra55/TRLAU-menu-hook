#pragma once

#include <functional>

#include "Instance.h"

class Instances
{
public:
	static void Init();

	static void Iterate(std::function<void(Instance*)> callback);
	static void Post(Instance* instance, int message, int data);
};