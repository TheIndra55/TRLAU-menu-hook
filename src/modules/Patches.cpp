#include <Hooking.Patterns.h>
#include <MinHook.h>
#include <jitasm.h>
#include <thread>

#include "Patches.h"
#include "util/Hooking.h"
#include "game/Game.h"
#include "MainMenu.h"
#include "patches/Reloc.h"
#include "patches/ErrorHandler.h"
#include "patches/Multicore.h"
#include "game/Camera.h"
#include "render/Draw.h"
#include "file/FileSystem.h"

// Instance of patches so we can get it in our hooks without calling GetModule<T> each call
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

		return;
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

static void TransToDrawVertexV4f(DRAWVERTEX* v, cdc::Vector3* vec)
{
	auto camera = CAMERA_GetCamera();
	auto transform = Game::IsInNextGenMode() ? camera->wcTransform2f : camera->wcTransformf;

	v->x = transform.col0.x * vec->x + transform.col1.x * vec->y + transform.col2.x * vec->z + transform.col3.x;
	v->y = transform.col0.y * vec->x + transform.col1.y * vec->y + transform.col2.y * vec->z + transform.col3.y;
	v->z = transform.col0.z * vec->x + transform.col1.z * vec->y + transform.col2.z * vec->z + transform.col3.z;
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

#ifdef TR8
static void(*s_MAIN_DoMainInit)();

static unsigned char CPUCount(unsigned int* TotAvailLogical, unsigned int* TotAvailCore, unsigned int* PhysicalNum)
{
	*TotAvailLogical = std::thread::hardware_concurrency();

	return 0;
}

static void MAIN_DoMainInit()
{
	s_MAIN_DoMainInit();

	// Patch the player list once globalInfo has been loaded
	s_patches->PatchPlayersList();
}
#endif

Patches::Patches()
{
	s_patches = this;
	s_menu = Hook::GetInstance().GetModule<MainMenu>().get();

#ifndef TR8
	if (m_disableIntro.GetValue() > Disabled)
	{
		RemoveIntro();
	}

	// Insert hooks
	MH_CreateHook((void*)GET_ADDRESS(0x40CA80, 0x43AB40, 0x000000), RenderG2_MotionBlur, (void**)&s_RenderG2_MotionBlur);
	MH_CreateHook((void*)GET_ADDRESS(0x450430, 0x452A90, 0x000000), GAMELOOP_HandleScreenWipes, (void**)&s_GAMELOOP_HandleScreenWipes);

	// Insert reloc hook
	MH_CreateHook((void*)GET_ADDRESS(0x4642F0, 0x467E60, 0x000000), MakePeHandle, nullptr);
#endif

	// Insert error handler hook
	MH_CreateHook((void*)GET_ADDRESS(0x401EE0, 0x401DF0, 0x478B00), MainG2_ErrorHandler, nullptr);

	// Insert DeathState hooks
	MH_CreateHook((void*)GET_ADDRESS(0x55DEC0, 0x5581D0, 0x75AA50), DeathState_Entry, (void**)&s_DeathState_Entry);
	MH_CreateHook((void*)GET_ADDRESS(0x56EC70, 0x5699C0, 0x75AF90), DeathState_Process, (void**)&s_DeathState_Process);

	// NOP the original death wipe code in DeathState::Entry
	Hooking::Nop((void*)GET_ADDRESS(0x55E188, 0x5584DC, 0x75AEDE), 5);

#ifdef TR7
	// NOP the exception handler in Legend
	Hooking::Nop((void*)0x401F53, 26);

	// Fix issues with dynamic drawing when using next generation graphics
	MH_CreateHook((void*)0x402EF0, TransToDrawVertexV4f, nullptr);

	// Fix issue with some broken models in next generation graphics reading out of bounds
	PatchDrawableHelper();
#endif

	// Patches
	if (m_heapSize.GetValue() > 0)
	{
		PatchHeapSize();
	}

#ifdef TR8
	// Fix cdcMultiCore breaking on high number of CPU cores
	MH_CreateHook((void*)0x4A21D0, CPUCount, nullptr);
	MH_CreateHook((void*)0x4A2680, cdc::JobChainImplWithThreads::StartSystem, (void**)&cdc::JobChainImplWithThreads::s_StartSystem);

	MH_CreateHook((void*)0x5DEF70, MAIN_DoMainInit, (void**)&s_MAIN_DoMainInit);
#endif

#ifdef TR7
	if (m_shadowMapSize.GetValue() > 0)
	{
		PatchShadowMap();
	}
#endif

	MH_EnableHook(MH_ALL_HOOKS);
}

void Patches::RemoveIntro() const noexcept
{
	auto match = hook::pattern("8D 0C 8D 03 00 00 00 89 0D").count(1);
	auto mainState = *match.get_first<int>(9);

	// Set new state to either main menu or intros
	auto newState = m_disableIntro.GetValue() == SkipIntros ? 6 : 3;

	// Nop out the code setting the main state to intros
	Hooking::Nop(match.get_first(), 13);

	// mov [mainState], newState
	Hooking::Patch(match.get_first(), { 0xC7, 0x05, 0x00, 0x00, 0x00, 0x00 });
	Hooking::Patch(match.get_first(2), mainState);
	Hooking::Patch(match.get_first(6), newState);
}

void Patches::PatchHeapSize() const noexcept
{
	auto match = hook::pattern("68 00 00 00 10 6A 00 C7 06").count(1);
	auto size = m_heapSize.GetValue();

	Hooking::Patch(match.get_first(1), size);
	Hooking::Patch(match.get_first(19), size);
}

#ifdef TR7
void Patches::PatchShadowMap() const noexcept
{
	auto match = hook::pattern("BF 00 04 00 00 3B C7 8B F1 BA 80 00 00 00").count(1);

	Hooking::Patch(match.get_first(1), m_shadowMapSize.GetValue());
}

void Patches::PatchDrawableHelper() const noexcept
{
	auto match = hook::pattern("05 ? ? ? ? 0F 28 08 0F 58 C1").count_hint(1);

	if (match.empty())
	{
		return;
	}

	static struct : jitasm::Frontend
	{
		void InternalMain()
		{
			cmp(eax, 256 * 16);		// Check for overflow on pivots
			jg("fail");
			add(eax, 0x113F880);	// Original code

			mov(edx, 0x5C136B);		// Jump to normal code
			jmp(edx);

			L("fail");
			mov(edx, 0x5C1505);		// Jump to end of function
			jmp(edx);
		}
	} stub;

	Hooking::Jump(match.get_first(), stub.GetCode());
}
#endif

#ifdef TR8
// Iterates the player list from globalInfo and removes any player objects that don't actually exist
// rather than using a hardcoded list this actually checks if it exists to account for mods
void Patches::PatchPlayersList() const noexcept
{
	auto globalInfo = *(GlobalInfo**)0xE7EE50;
	auto players = globalInfo->playerObjects;

	auto fileSystem = GetFS();
	char fileName[256];

	for (int i = 0; i < players->numPlayerObjects; )
	{
		auto name = OBTABLE_GetObjectName(players->playerObjectList[i]);

		LOAD_UnitFileName(fileName, name, "drm");

		// Check if the object exist
		if (!fileSystem->FileExists(fileName))
		{
			// If the file does not exist, remove it from the list
			for (int j = i; j < players->numPlayerObjects - 1; j++) players->playerObjectList[j] = players->playerObjectList[j + 1];

			players->numPlayerObjects--;
		}
		else
		{
			i++;
		}
	}
}
#endif

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