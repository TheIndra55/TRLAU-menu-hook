#include "Instance.h"

#include "util/Hooking.h"

void INSTANCE_Post(Instance* instance, int message, int data)
{
	Hooking::Call(0x4580B0, instance, message, data);
}

void INSTANCE_HideUnhideDrawGroup(Instance* instance, int drawGroup, int on)
{
	Hooking::Call(0x4319B0, instance, drawGroup, on);
}