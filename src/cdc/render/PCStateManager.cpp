#include "PCStateManager.h"

#include "util/Hooking.h"

void cdc::PCStateManager::SetRenderState(D3DRENDERSTATETYPE state, unsigned int value)
{
	Hooking::ThisCall(0x502B70, this, state, value);
}