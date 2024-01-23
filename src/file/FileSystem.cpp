#include "FileSystem.h"
#include "util/Hooking.h"

cdc::FileSystem* GetFS()
{
	auto addr = GET_ADDRESS(0x45C700, 0x45F640, 0x472B50);

	return Hooking::CallReturn<cdc::FileSystem*>(addr);
}