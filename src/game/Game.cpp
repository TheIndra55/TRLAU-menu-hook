#include "Game.h"

#include "util/Hooking.h"
#include "game/NtUnlockableCostume.h"

Instance* Game::GetPlayerInstance() noexcept
{
	return GetGameTracker()->playerInstance;
}

GameTracker* Game::GetGameTracker() noexcept
{
	return (GameTracker*)GET_ADDRESS(0x10E5370, 0x838330, 0xE7F088);
}

STracker* Game::GetStreamTracker() noexcept
{
	return (STracker*)GET_ADDRESS(0x11582F8, 0x8AE378, 0xDBAB40);
}

void Game::SwitchPlayerCharacter() noexcept
{
#ifndef TR8
	PLAYER_DebugSwitchPlayerCharacter();
#else
	// Switch the player costume with costume index 12, this costume has an empty object id
	// which allows us to fallback to the default behavior
	NtUnlockableCostume costume;
	costume.costumeIndex = 12;

	NtUnlockableCostume_ScriptType::SwitchPlayerCostume(&costume);

	// Post to birth the player weapons
	INSTANCE_Post(Game::GetPlayerInstance(), 35, 0);
#endif
}

bool Game::IsInNextGenMode() noexcept
{
	return *(bool*)GET_ADDRESS(0x10024E8, 0x7545B4, 0x000000);
}

void GAMELOOP_RequestLevelChangeByName(char* name, GameTracker* gameTracker, int doneType)
{
	auto addr = GET_ADDRESS(0x451970, 0x4543C0, 0x5DF8C0);

	Hooking::Call(addr, name, gameTracker, doneType);
}

bool GAMELOOP_IsWipeDone(int type)
{
	auto addr = GET_ADDRESS(0x450310, 0x452970, 0x52E880);

	return Hooking::CallReturn<bool>(addr, type);
}

void GAMELOOP_SetScreenWipe(int type, int target, int time)
{
	auto addr = GET_ADDRESS(0x4503D0, 0x452A30, 0x52E8B0);

	Hooking::Call(addr, type, target, time);
}

void PLAYER_DebugSwitchPlayerCharacter()
{
	auto addr = GET_ADDRESS(0x5A40B0, 0x5A39A0, 0x79DB50);

	Hooking::Call(addr);
}

int OBTABLE_GetObjectID(char* name)
{
	auto addr = GET_ADDRESS(0x462590, 0x4660C0, 0x5BF770);

	return Hooking::CallReturn<int>(addr, name);
}

char* OBTABLE_GetObjectName(int id)
{
	auto objectList = *(ObjectList**)GET_ADDRESS(0x000000, 0x000000, 0xDB94D0);

	return objectList->object[id - 1].name;
}

void LOAD_ObjectFileName(char* name, char* object, char* extension)
{
	return LOAD_UnitFileName(name, object, extension);
}

void LOAD_UnitFileName(char* name, char* unit, char* extension)
{
	auto addr = GET_ADDRESS(0x45C730, 0x45F650, 0x477970);

	return Hooking::Call(addr, name, unit, extension);
}