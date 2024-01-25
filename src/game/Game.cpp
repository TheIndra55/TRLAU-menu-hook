#include "Game.h"

#include "util/Hooking.h"

Instance* Game::GetPlayerInstance()
{
	return GetGameTracker()->playerInstance;
}

GameTracker* Game::GetGameTracker()
{
	return (GameTracker*)GET_ADDRESS(0x10E5370, 0x838330, 0x00E7F088);
}

STracker* Game::GetStreamTracker()
{
	return (STracker*)GET_ADDRESS(0x11582F8, 0x8AE378, 0xDBAB40);
}

void GAMELOOP_RequestLevelChangeByName(char* name, GameTracker* gameTracker, int doneType)
{
	auto addr = GET_ADDRESS(0x451970, 0xC61CFA, 0x5DF8C0);

	Hooking::Call(addr, name, gameTracker, doneType);
}

void PLAYER_DebugSwitchPlayerCharacter()
{
	auto addr = GET_ADDRESS(0x5A40B0, 0x5A39A0, 0x79DB50);

	Hooking::Call(addr);
}

int OBTABLE_GetObjectID(char* name)
{
	auto addr = GET_ADDRESS(0x462590, 0x465DE0, 0x5BF770);

	return Hooking::CallReturn<int>(addr, name);
}