#include "Markup.h"
#include "util/Hooking.h"

MarkupManager* MarkupManager::GetInstance() noexcept
{
	return *(MarkupManager**)GET_ADDRESS(0x1117544, 0x86CD14, 0xDB94CC);
}

cdc::Vector3* MarkUpBox::GetSegmentPos(cdc::Vector3* result, int n) const noexcept
{
	auto addr = GET_ADDRESS(0x45F3A0, 0x462D90, 0x5C39D0);

	return Hooking::ThisCallReturn<cdc::Vector3*>(addr, this, result, n);
}