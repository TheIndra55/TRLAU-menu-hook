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

void GAMELOOP_RequestLevelChangeByName(char* name, GameTracker* gameTracker, int doneType)
{
	auto addr = GET_ADDRESS(0x451970, 0xC61CFA, 0x5DF8C0);

	Hooking::Call(addr, name, gameTracker, doneType);
}