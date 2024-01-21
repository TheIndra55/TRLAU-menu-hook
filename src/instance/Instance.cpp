#include "Instance.h"

#include "util/Hooking.h"

void INSTANCE_Post(Instance* instance, int message, int data)
{
	auto addr = GET_ADDRESS(0x455510, 0x4580B0, 0x5B3750);

	Hooking::Call(addr, instance, message, data);
}

void INSTANCE_HideUnhideDrawGroup(Instance* instance, int drawGroup, int on)
{
	auto addr = GET_ADDRESS(0x456230, 0x4319B0, 0x5B3DD0);

	Hooking::Call(addr, instance, drawGroup, on);
}

void INSTANCE_ReallyRemoveInstance(Instance* instance, int reset, bool keepSave)
{
	auto addr = GET_ADDRESS(0x4575B0, 0x45A3A0, 0x5BC4E0);

	Hooking::Call(addr, instance, reset, keepSave);
}

Instance* INSTANCE_BirthObjectNoParent(unsigned int unitID, cdc::Vector3* position, cdc::Euler* rotation, IntroData* introData, Object* object, int modelNum, int initEffects)
{
	auto addr = GET_ADDRESS(0x458990, 0x45BA90, 0x5BD0F0);

	return Hooking::CallReturn<Instance*>(addr, unitID, position, rotation, introData, object, modelNum, initEffects);
}