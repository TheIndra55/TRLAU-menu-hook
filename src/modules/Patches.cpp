#include <Hooking.Patterns.h>
#include <MinHook.h>

#include "Patches.h"
#include "util/Hooking.h"
#include "game/Game.h"
#include "MainMenu.h"
#include "patches/Reloc.h"

// Instance of patches so we can get it in our hooks without calling GetModule<T>
static Patches* s_patches;
static MainMenu* s_menu;

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

// No death fade code
int(__stdcall* s_DeathState_Entry)(int player, int data);
void(__stdcall* s_DeathState_Process)(int player, int data);

static int __stdcall DeathState_Entry(int player, int data)
{
	auto ret = s_DeathState_Entry(player, data);

	if (!s_menu->IsNoDeathFade())
	{
		// We NOPed the original code, so we call the wipe manually
		// like the game code does
		if (GAMELOOP_IsWipeDone(10))
		{
			// TODO refactor, 90 is not a constant value in the game code
			GAMELOOP_SetScreenWipe(10, 100, 90);
		}
	}

	return ret;
}

static void __stdcall DeathState_Process(int player, int data)
{
	if (!s_menu->IsNoDeathFade())
	{
		s_DeathState_Process(player, data);
	}
}

Patches::Patches()
{
	s_patches = this;
	s_menu = Hook::GetInstance().GetModule<MainMenu>().get();

#ifndef TR8
	if (m_disableIntro.GetValue())
	{
		RemoveIntro();
	}

	// Insert hooks
	MH_CreateHook((void*)GET_ADDRESS(0x40CA80, 0x43AB40, 0x000000), RenderG2_MotionBlur, (void**)&s_RenderG2_MotionBlur);
	MH_CreateHook((void*)GET_ADDRESS(0x450430, 0x452A90, 0x000000), GAMELOOP_HandleScreenWipes, (void**)&s_GAMELOOP_HandleScreenWipes);

	// Insert reloc hook
	MH_CreateHook((void*)GET_ADDRESS(0x4642F0, 0x467E60, 0x000000), MakePeHandle, nullptr);
#endif

	// Insert DeathState hooks
	MH_CreateHook((void*)GET_ADDRESS(0x55DEC0, 0x5581D0, 0x75AA50), DeathState_Entry, (void**)&s_DeathState_Entry);
	MH_CreateHook((void*)GET_ADDRESS(0x56EC70, 0x5699C0, 0x75AF90), DeathState_Process, (void**)&s_DeathState_Process);

	// NOP the original death wipe code in DeathState::Entry
	Hooking::Nop((void*)GET_ADDRESS(0x55E188, 0x5584DC, 0x75AEDE), 5);

	MH_EnableHook(MH_ALL_HOOKS);
}

void Patches::RemoveIntro() const noexcept
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