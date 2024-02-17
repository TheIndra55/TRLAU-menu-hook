#include "Event.h"
#include "util/Hooking.h"

void* RELOC_GetProcAddress(void* peData, char* lpszName)
{
	auto addr = GET_ADDRESS(0x464550, 0x4680C0, 0x000000);

	return Hooking::CallReturn<void*>(addr, peData, lpszName);
}