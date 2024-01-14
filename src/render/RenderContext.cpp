#include <Windows.h>

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
		MH_CreateHook((void*)0x61BB80, Present, (void**)&s_Present);
		MH_EnableHook(MH_ALL_HOOKS);
	}

	s_callback = callback;
}