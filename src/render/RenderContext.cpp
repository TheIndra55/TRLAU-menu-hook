#include <Windows.h>
#include <Hooking.Patterns.h>
#include <MinHook.h>

#include "RenderContext.h"

static std::function<void()> s_callback;

// cdc::PCRenderContext::Present
static void(__thiscall* s_Present)(void*, RECT*, RECT*, HWND);

static void __fastcall Present(void* _this, void*, RECT* pSourceRect, RECT* pDestRect, HWND hOverrideWND)
{
	s_callback();
	s_Present(_this, pSourceRect, pDestRect, hOverrideWND);
}

void RenderContext::OnPresent(std::function<void()> callback)
{
	if (!s_callback)
	{
#ifndef TR8
		auto match = hook::pattern("8B 41 14 85 C0 74 19 8B 54 24 0C").count(1);
#else
		auto match = hook::pattern("A1 ? ? ? ? 83 78 10 00 75 39 80 B8").count(1);
#endif

		MH_CreateHook(match.get_first(), Present, (void**)&s_Present);
		MH_EnableHook(MH_ALL_HOOKS);
	}

	s_callback = callback;
}