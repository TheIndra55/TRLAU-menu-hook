#include <MinHook.h>

#include "GameLoop.h"
#include "Game.h"
#include "util/Hooking.h"

static std::function<void()> s_callback;
static void(*s_GAMELOOP_Process)(GameTracker*);

static void GAMELOOP_Process(GameTracker* gameTracker)
{
	s_callback();
	s_GAMELOOP_Process(gameTracker);
}

void GameLoop::OnLoop(std::function<void()> callback) noexcept
{
	if (!s_callback)
	{
		MH_CreateHook((void*)GET_ADDRESS(0x452140, 0x454AC0, 0x5DFBE0), GAMELOOP_Process, (void**)&s_GAMELOOP_Process);
		MH_EnableHook(MH_ALL_HOOKS);
	}

	s_callback = callback;
}