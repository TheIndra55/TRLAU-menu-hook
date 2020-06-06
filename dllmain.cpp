#include <d3d9.h>

#include "pch.h"

#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"

#include "MinHook.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static LPDIRECT3DDEVICE9 g_pd3dDevice;
static bool              g_focus = false;

static int               g_gameTracker = 0x838330;

// game function pointers
static char(__cdecl* g_LoadChapter)(char*);

static char(*g_ReloadLevel)();

static char(__cdecl* g_ResetGame)(int);

void GetPosition(float& x, float& y, float& z)
{
    auto base = *(DWORD*)0x83833C;

    // return whenever pointer isn't initialized yet
    if (base == 0) return;

    x = *(float*)(base + 0x10);
    y = *(float*)(base + 0x14);
    z = *(float*)(base + 0x18);
}

// orginal function pointers and hooks
static void(__thiscall* g_cdc_PCRenderContext_Present)(DWORD*, int, int, int);

void __fastcall cdc_PCRenderContext_Present(DWORD* _this, void* _, int a2, int a3, int a4)
{
    // show cursor whenever foccused
    ImGui::GetIO().MouseDrawCursor = g_focus;

    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    static char chapter[32] = "";
    static char unit[32] = "";

    ImGui::Begin("Menu", nullptr);

    // show current unit
    ImGui::Text("Unit: %s", (char*)(g_gameTracker + 204));

    ImGui::InputText("chapter", chapter, 32);
    ImGui::InputText("unit", unit, 32);

    if (ImGui::Button("Load chapter"))
    {
        // load chapter (chapter0, chapter1..)
        // chapter0 = croft manor
        g_LoadChapter(chapter);
    }
    if (ImGui::Button("Load unit"))
    {
        // change current unit
        strcpy_s((char*)(g_gameTracker + 204), 32, unit);
        g_ResetGame(4);
    }
    if (ImGui::Button("Reload unit"))
    {
        g_ReloadLevel();
    }

    float x, y, z;
    GetPosition(x, y, z);

    // position based on unit?
    ImGui::Text("Position: %f, %f, %f", x, y, z);

    ImGui::End();

    ImGui::EndFrame();

    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

    // call orginal Present function to render the frame
    g_cdc_PCRenderContext_Present(_this, a2, a3, a4);
}

static LRESULT(__stdcall *g_RegularWndProc)(HWND, UINT, WPARAM, LPARAM);

static LRESULT RegularWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    if (msg == WM_KEYUP && wparam == VK_F8)
    {
        g_focus = !g_focus;
    }

    if (g_focus)
    {
        ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam);
    }

    // pass input to orginal game wndproc
    return g_RegularWndProc(hwnd, msg, wparam, lparam);
}

static BOOL(WINAPI *g_SetCursorPos)(int, int);

static BOOL WINAPI hSetCursorPos(int x, int y)
{
    // prevent game from reseting cursor position
    if (!g_focus)
    {
        return g_SetCursorPos(x, y);
    }

    return 1;
}

DWORD WINAPI Draw(LPVOID lpParam)
{
    MH_Initialize();

    // hook before the game calls ->Present so we can render just before the frame ends
    MH_CreateHook((void*)0x61BB80, cdc_PCRenderContext_Present, reinterpret_cast<void**>(&g_cdc_PCRenderContext_Present));

    // hook the game's wndproc so we can pass input to imgui
    MH_CreateHook((void*)0x4040B0, RegularWndProc, reinterpret_cast<void**>(&g_RegularWndProc));

    // hook SetCursorPos to prevent the game from resetting the cursor position
    MH_CreateHookApi(L"user32", "SetCursorPos", hSetCursorPos, reinterpret_cast<void**>(&g_SetCursorPos));

    // give the game some time to init d3d9 device and window
    // TODO refactor, hook just after creation
    Sleep(5000);

    // get the game window handle
    HWND hwnd = *(HWND*)0x6926C8;

    // get the draw device pointer
    auto address = *(DWORD*)0xA6669C;
    auto device = *(DWORD*)(address + 0x20);

    g_LoadChapter = (char(__cdecl*)(char*))0x422090;
    g_ReloadLevel = (char(*)())0x4279F0;
    g_ResetGame = (char(__cdecl*)(int))0x004542B0;

    g_pd3dDevice = (IDirect3DDevice9*)device;

    // setup imgui
    ImGui::CreateContext();

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX9_Init(g_pd3dDevice);

    // start all hooks and start rendering
    MH_EnableHook(MH_ALL_HOOKS);

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
        CreateThread(nullptr, 0, Draw, NULL, 0, NULL);

        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

