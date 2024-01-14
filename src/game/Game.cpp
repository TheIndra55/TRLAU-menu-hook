#include "Game.h"

void Game::Init()
{
}

Instance* Game::GetPlayerInstance()
{
	return GetGameTracker()->playerInstance;
}

GameTracker* Game::GetGameTracker()
{
	return (GameTracker*)0x838330;
}