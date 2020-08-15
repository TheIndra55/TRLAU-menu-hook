#define STEAM 1

#if STEAM
    #define _CRT_SECURE_NO_WARNINGS
#endif

#include "pch.h"
#include "Hooking.hpp"

#if STEAM
    #define _CRT_SECURE_NO_WARNINGS
    #include "steam_api.h"
#endif

Hooking* g_hooking;

DWORD WINAPI Hook(LPVOID lpParam)
{

#if STEAM
    // hardcoded path, might differ
    LoadLibraryA("c:\\Program Files (x86)\\Steam\\gameoverlayrenderer.dll");
    SteamAPI_Init();
#endif

    g_hooking = new Hooking();
    g_hooking->Initialize();

    // don't exit thread
    while (true) Sleep(0);

    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
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

