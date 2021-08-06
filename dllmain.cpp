#include "Hooking.hpp"

static bool hit = false;

BOOL (WINAPI* dGetVersionExA)(LPOSVERSIONINFOA  lpStartupInfo);

BOOL  WINAPI hGetVersionExA(LPOSVERSIONINFOA  lpStartupInfo)
{
    if (!hit)
    {
        Hooking::GetInstance(); // Will call the ctor
        hit = true;

        // TODO MH_RemoveHook
    }

    return dGetVersionExA(lpStartupInfo);
}

DWORD WINAPI Hook(LPVOID lpParam)
{
    MH_Initialize();

#if TRAE || TR7
    // we cannot insert our hooks now since game is not done yet unpacking
    // hook one of the first functions called from unpacked code and insert our hooks then
    MH_CreateHookApi(L"Kernel32", "GetStartupInfoW", hGetVersionExA, reinterpret_cast<void**>(&dGetVersionExA));
    MH_EnableHook(MH_ALL_HOOKS);
#else
    Hooking::GetInstance(); // Will call the ctor
#endif

    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
            CreateThread(nullptr, 0, Hook, NULL, 0, NULL);
            break;
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
            break;
    }

    return TRUE;
}

