#include "FileSystem.h"
#include "util/Hooking.h"

cdc::FileSystem* GetFS() noexcept
{
	auto addr = GET_ADDRESS(0x45C700, 0x45F640, 0x472B50);

	return Hooking::CallReturn<cdc::FileSystem*>(addr);
}

cdc::FileSystem* GetDiskFS() noexcept
{
	return *(cdc::FileSystem**)GET_ADDRESS(0x10E58C0, 0x838890, 0x9CE27C);
}