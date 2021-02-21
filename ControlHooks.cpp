#include <stdio.h>
#include "include/MinHook.h"
#include "Hooking.hpp"

//int GetNumButtons()
//{
//	return 21 /* game default */;
//}

char(__thiscall* UIDataList__AllocateRunData)(DWORD _this, int a2, int a3, int a4, int a5, int a6);

char(__thiscall* UIDataList__AddItem)(DWORD _this, int processFunc, void* data, int getDataFunc, int a5, const char* a6);

const char* GetControlText(void* data, int index, int textIndex)
{
	if (textIndex)
	{
		return "DEL";
	}

	return "Binoculars";
}

char UIControllerConfigDataList(int a1, int a2)
{
	// since the game controller config is complex and hardcoded array size
	// we construct the controller config data list ourself and add our own control

	if (!UIDataList__AllocateRunData(a1, 22, a2, 0x4EC5E0, 0x4EC5F0, 0x4ECB10))
		return 1;

	for (int i = 0; i < 21; i++)
	{
		UIDataList__AddItem(a1, 0x4ECB30, (void*)i, 0x4ECBE0, 0, 0);
	}

	// hardcoded for now since internal controls array is a fixed size
	UIDataList__AddItem(a1, 0, 0, (int)GetControlText, 0, 0);

	return 1;
}

void InstallControlHooks()
{
	//MH_CreateHook((void*)0x004EC280, GetNumButtons, nullptr);

	UIDataList__AllocateRunData = reinterpret_cast<char(__thiscall*)(DWORD, int, int, int, int, int)>(0x00C704C2);
	UIDataList__AddItem = reinterpret_cast<char(__thiscall*)(DWORD, int, void*, int, int, const char*)>(0x00C70577);

	MH_CreateHook((void*)0x004ECCB0, UIControllerConfigDataList, nullptr);

	MH_EnableHook(MH_ALL_HOOKS);
}