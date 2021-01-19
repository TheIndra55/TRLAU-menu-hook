#pragma once
#include "MinHook.h"
#include "Menu.hpp"

class Hooking
{
public:
	void Initialize();
	void Uninitialize();

	void GotDevice();

	Menu* menu;
};

static int(*original_Direct3DInit)();
static int hooked_Direct3DInit();

static void(__thiscall* original_PCRenderContext_Present)(DWORD*, int, int, int);
void __fastcall hooked_PCRenderContext_Present(DWORD* _this, void* _, int a2, int a3, int a4);

static LRESULT(__stdcall* original_RegularWndProc)(HWND, UINT, WPARAM, LPARAM);
static LRESULT hooked_RegularWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

static BOOL(WINAPI* original_SetCursorPos)(int, int);
static BOOL WINAPI hooked_SetCursorPos(int x, int y);

// recycled from another unfinished project of mine, needs more research
namespace cdc
{
	struct Vector3
	{
		float x, y, z;
		float unused;
	};
}

struct Instance
{
	__int64 unk1;
	__int32 next;
	__int32 prev;
	cdc::Vector3 position;
	cdc::Vector3 prevPosition;
	cdc::Vector3 rotation;
	cdc::Vector3 prevRotation;
	cdc::Vector3 scale;
	cdc::Vector3 shadowPosition;
	cdc::Vector3 unk4;
	__int32 unk5;
	__int32 unk6;
	__int16 unk7;
	__int8 unk8;
	__int8 unk9;
	__int16 unk10;
	DWORD* object;
};