#include "Instances.h"

void Instances::Iterate(std::function<void(Instance*)> callback)
{
	auto first = *(Instance**)0x817D64;

	for (auto instance = first; instance != nullptr; instance = instance->next)
	{
		callback(instance);
	}
}