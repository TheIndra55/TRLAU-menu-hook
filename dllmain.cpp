#include "Hooking.hpp"

static bool hit = false;

BOOL (WINAPI* dGetVersionExA)(LPSTARTUPINFOA lpStartupInfo);

BOOL  WINAPI hGetVersionExA(LPSTARTUPINFOA lpStartupInfo)
{
    if (!hit)
    {
        // insert this hook early
#if TRAE
        MH_CreateHook((void*)0x45F640, GetFS, nullptr);
#elif TR7
        MH_CreateHook((void*)ADDR(0x45F420, 0x45C700), GetFS, nullptr);
#endif

        MH_EnableHook(GetFS);

        Hooking::GetInstance(); // Will call the ctor
        hit = true;

        // TODO MH_RemoveHook
    }

    return dGetVersionExA(lpStartupInfo);
}

bool CheckVersion()
{
    auto hModule = GetModuleHandleA(nullptr);
    auto header = (PIMAGE_NT_HEADERS)((DWORD_PTR)hModule + ((PIMAGE_DOS_HEADER)hModule)->e_lfanew);

#if RETAIL_VERSION
    // if retail check if not debug exe timestamp
    return header->FileHeader.TimeDateStamp != 0x446DCF16;
#else
    // if debug check for debug exe timestamp
    return header->FileHeader.TimeDateStamp == 0x446DCF16;
#endif
}

DWORD WINAPI Hook(LPVOID lpParam)
{
    MH_Initialize();

#if TRAE || TR7
    // we cannot insert our hooks now since game is not done yet unpacking
    // hook one of the first functions called from unpacked code and insert our hooks then
    MH_CreateHookApi(L"Kernel32", "GetStartupInfoA", hGetVersionExA, reinterpret_cast<void**>(&dGetVersionExA));
    MH_EnableHook(MH_ALL_HOOKS);
#else
    MH_CreateHook((void*)0x478930, InitPatchArchive, (void**)&orgInitPatchArchive);
    MH_EnableHook(InitPatchArchive);

    Hooking::GetInstance(); // Will call the ctor
#endif

    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    // since there could be retail and debug asi in same folder
    // unload early if exe timestamp is wrong one
#if TR7
    if (!CheckVersion())
    {
        return FALSE;
    }
#endif

    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
            //CreateThread(nullptr, 0, Hook, NULL, 0, NULL);
            Hook(NULL);
            break;
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
            break;
    }

    return TRUE;
}

