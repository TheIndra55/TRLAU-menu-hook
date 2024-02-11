#include "Game.h"
#include "util/Hooking.h"

void IncrHealth(float amount)
{
	auto addr = GET_ADDRESS(0x576CA0, 0x5715E0, 0x000000);

	Hooking::Call(addr, amount);
}