#include "Game.h"

#include "util/Hooking.h"

Instance* Game::GetPlayerInstance()
{
	return GetGameTracker()->playerInstance;
}

GameTracker* Game::GetGameTracker()
{
	return (GameTracker*)0x838330;
}

void GAMELOOP_ExitGame(char* name, GameTracker* gameTracker, int doneType)
{
	Hooking::Call(0xC61CFA, name, gameTracker, doneType);
}