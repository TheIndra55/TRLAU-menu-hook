#include <Windows.h>
#include <MinHook.h>

#include "MouseHook.h"

static bool s_disabled;
static BOOL(WINAPI* s_SetCursorPos)(int, int);

BOOL WINAPI SetCursorPosHook(int X, int Y)
{
	if (s_disabled)
	{
		return TRUE;
	}

	return s_SetCursorPos(X, Y);
}

void MouseHook::Init() noexcept
{
	MH_CreateHookApi(L"user32", "SetCursorPos", SetCursorPosHook, (void**)&s_SetCursorPos);
	MH_EnableHook(MH_ALL_HOOKS);
}

void MouseHook::DisableCursorLock(bool disable) noexcept
{
	s_disabled = disable;
}