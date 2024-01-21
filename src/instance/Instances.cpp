#include "Instances.h"
#include "util/Hooking.h"

void Instances::Iterate(std::function<void(Instance*)> callback)
{
	auto first = *(Instance**)GET_ADDRESS(0x10C5AA4, 0x817D64, 0xD98D54);

	for (auto instance = first; instance != nullptr; instance = instance->next)
	{
		callback(instance);
	}
}