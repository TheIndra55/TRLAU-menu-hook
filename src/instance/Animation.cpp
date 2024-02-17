#include "Animation.h"
#include "util/Hooking.h"

void G2EmulationInstanceSetAnimation(Instance* instance, int CurrentSection, int NewAnim, int NewFrame, int Frames)
{
	auto addr = GET_ADDRESS(0x4DEC30, 0x4DE690, 0x5B1EA0);

	Hooking::Call(addr, instance, CurrentSection, NewAnim, NewFrame, Frames);
}

void G2EmulationInstanceSetMode(Instance* instance, int CurrentSection, int Mode)
{
	auto addr = GET_ADDRESS(0x4DED90, 0x4DE7F0, 0x5B1F50);

	Hooking::Call(addr, instance, CurrentSection, Mode);
}

int G2EmulationInstanceQueryAnimation(Instance* instance, int CurrentSection)
{
	// TODO Underworld
	auto addr = GET_ADDRESS(0x4DEE50, 0x4DE8B0, 0x000000);

	return Hooking::CallReturn<int>(addr, instance, CurrentSection);
}