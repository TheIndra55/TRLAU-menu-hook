#include <Windows.h>
#include <MinHook.h>

#include "Hook.h" 

static void* s_target = nullptr;
static void (WINAPI* s_GetStartupInfoA)(LPSTARTUPINFOA);

static void WINAPI StartupInfoW(LPSTARTUPINFOA lpStartupInfo)
{
	// Initialize
	auto& hook = Hook::GetInstance();
	hook.Initialize();

	// Call the original GetStartupInfoA
	s_GetStartupInfoA(lpStartupInfo);

	// Remove the hook
	MH_RemoveHook(s_target);
}

static void Initialize()
{
	// Initialize MinHook
	MH_Initialize();

	// We insert an early hook on GetStartupInfoA to know when the executable is done unpacking
	// some executables are packed/protected so we can't insert our hooks yet.
	MH_CreateHookApiEx(L"kernel32", "GetStartupInfoA", StartupInfoW, (void**)&s_GetStartupInfoA, &s_target);
	MH_EnableHook(MH_ALL_HOOKS);
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	switch (fdwReason)
	{
		case DLL_PROCESS_ATTACH:
			Initialize();

			break;
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
	}

	return TRUE;
}