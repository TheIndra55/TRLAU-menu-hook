#include "Stream.h"
#include "util/Hooking.h"

ObjectTracker* STREAM_GetObjectTrackerByName(char* name)
{
	auto addr = GET_ADDRESS(0x5D4270, 0x5DA260, 0x5C17D0);

	return Hooking::CallReturn<ObjectTracker*>(addr, name);
}

bool STREAM_PollLoadQueue()
{
	auto addr = GET_ADDRESS(0x5D51C0, 0x5DB190, 0x5C1DA0);

	return Hooking::CallReturn<bool>(addr);
}