#include "Instances.h"

static void(*INSTANCE_Post)(Instance*, int, int);

void Instances::Init()
{
	INSTANCE_Post = reinterpret_cast<void(*)(Instance*, int, int)>(0x4580B0);
}

void Instances::Iterate(std::function<void(Instance*)> callback)
{
	auto first = *(Instance**)0x817D64;

	for (auto instance = first; instance != nullptr; instance = instance->next)
	{
		callback(instance);
	}
}

void Instances::Post(Instance* instance, int message, int data)
{
	INSTANCE_Post(instance, message, data);
}