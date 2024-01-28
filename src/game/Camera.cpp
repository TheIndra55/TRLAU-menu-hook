#include "Camera.h"
#include "util/Hooking.h"

Camera* CAMERA_GetCamera()
{
#ifndef TR8
	return (Camera*)GET_ADDRESS(0x000000, 0x850670, 0x000000);
#else
	return *(Camera**)0xE80534;
#endif
}

void CAMERA_CalcPosition(cdc::Vector3* position, cdc::Vector3* base, cdc::Euler* rotation, float distance)
{
	auto addr = GET_ADDRESS(0x48D8B0, 0x491320, 0x000000);

	Hooking::Call(addr, position, base, rotation, distance);
}

#ifdef TR8
void CAMERA_SetMode(int mode)
{
	Hooking::Call(0x5F39F0, mode);
}
#endif