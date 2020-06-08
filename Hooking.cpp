#include "Hooking.hpp"
#include "Game.hpp"

extern Hooking* g_hooking;

LPDIRECT3DDEVICE9 pDevice;
HWND pHwnd;

void Hooking::Initialize()
{
	MH_Initialize();
	
	// hook into d3d9 creation function and wait for a device
	MH_CreateHook(
		reinterpret_cast<void*>(0xC5C175),
		hooked_Direct3DInit, 
		reinterpret_cast<void**>(&original_Direct3DInit));

	Game::Initialize();

	MH_EnableHook(MH_ALL_HOOKS);
}

void Hooking::Uninitialize()
{
	MH_Uninitialize();
}

void Hooking::GotDevice()
{
	this->menu = new Menu(pDevice, pHwnd);

	// hook game's d3d9 present function and wndproc function
	MH_CreateHook(
		reinterpret_cast<void*>(0x61BB80), 
		hooked_PCRenderContext_Present, 
		reinterpret_cast<void**>(&original_PCRenderContext_Present));

	MH_CreateHook(
		reinterpret_cast<void*>(0x4040B0), 
		hooked_RegularWndProc, 
		reinterpret_cast<void**>(&original_RegularWndProc));

	// hook SetCursorPos to prevent the game from resetting the cursor position
	MH_CreateHookApi(L"user32", "SetCursorPos", hooked_SetCursorPos, reinterpret_cast<void**>(&original_SetCursorPos));

	MH_EnableHook(MH_ALL_HOOKS);
}

int hooked_Direct3DInit()
{
	// call orginal game function to init d3direct device
	auto val = original_Direct3DInit();

	// get the d3d9 device and hwnd
	pHwnd = *reinterpret_cast<HWND*>(0x6926C8);

	// (IDirect3DDevice*)devicemanager->d3device
	auto address = *reinterpret_cast<DWORD*>(0xA6669C);
	auto device = *reinterpret_cast<DWORD*>(address + 0x20);
	pDevice = reinterpret_cast<IDirect3DDevice9*>(device);

	g_hooking->GotDevice();

	return val;
}

void __fastcall hooked_PCRenderContext_Present(DWORD* _this, void* _, int a2, int a3, int a4)
{
	g_hooking->menu->OnPresent();

	// call orginal game present function to draw on the screen
	original_PCRenderContext_Present(_this, a2, a3, a4);
}

LRESULT hooked_RegularWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (msg == WM_KEYUP && wparam == VK_F8)
	{
		g_hooking->menu->m_focus = !g_hooking->menu->m_focus;
	}

	// pass input to menu
	g_hooking->menu->Process(hwnd, msg, wparam, lparam);

	// pass input to orginal game wndproc
	return original_RegularWndProc(hwnd, msg, wparam, lparam);
}

BOOL WINAPI hooked_SetCursorPos(int x, int y)
{
	// prevent game from reseting cursor position
	if (g_hooking->menu->m_focus)
	{
		return 1;
	}

	return original_SetCursorPos(x, y);
}