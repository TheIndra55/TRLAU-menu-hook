#include <Hooking.Patterns.h>
#include <MinHook.h>

#include "Patches.h"
#include "util/Hooking.h"
#include "game/Game.h"

// Instance of patches so we can get it in our hooks without calling GetModule<T>
static Patches* s_patches;

#ifndef TR8
// Original functions
static void(*s_RenderG2_MotionBlur)(int, int, int);
static void(*s_GAMELOOP_HandleScreenWipes)();

static void RenderG2_MotionBlur(int blur, int time, int timeout)
{
	if (s_patches->IsNoMotionBlur())
	{
		s_RenderG2_MotionBlur(0, 0, 0);
	}

	s_RenderG2_MotionBlur(blur, time, timeout);
}

static void GAMELOOP_HandleScreenWipes()
{
	if (s_patches->IsNoCinematicBars())
	{
		auto gameTracker = Game::GetGameTracker();

		// Set the bars wipe to 0, this is actually better than our previous approach
		// where we patched the code for wipe 11, since it was shared with wipe 6
		auto wipe = &gameTracker->wipes[11];
		wipe->wipeCur = 0.f;
		wipe->wipeTarget = 0.f;
	}

	s_GAMELOOP_HandleScreenWipes();
}
#endif

Patches::Patches()
{
	s_patches = this;

#ifndef TR8
	if (m_disableIntro.GetValue())
	{
		RemoveIntro();
	}

	// Insert hooks
	MH_CreateHook((void*)GET_ADDRESS(0x40CA80, 0x43AB40, 0x000000), RenderG2_MotionBlur, (void**)&s_RenderG2_MotionBlur);
	MH_CreateHook((void*)GET_ADDRESS(0x450430, 0x452A90, 0x000000), GAMELOOP_HandleScreenWipes, (void**)&s_GAMELOOP_HandleScreenWipes);
	MH_EnableHook(MH_ALL_HOOKS);
#endif
}

void Patches::RemoveIntro()
{
	auto match = hook::pattern("8D 0C 8D 03 00 00 00 89 0D").count(1);
	auto mainState = *match.get_first<int>(9);

	// Nop out the code setting the main state to intros
	Hooking::Nop(match.get_first(), 13);

	// mov [mainState], 6
	Hooking::Patch(match.get_first(), { 0xC7, 0x05, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00 });
	Hooking::Patch(match.get_first(2), mainState);
}

void Patches::OnInput(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// Remove the quit message
#ifdef TRAE
	if (msg == WM_CLOSE)
	{
		PostQuitMessage(0);
	}
#endif
}