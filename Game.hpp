#pragma once
#include <functional>
#include "pch.h"

#define GAMETRACKER 0x838330
#define PLAYERINSTANCE 0x83833C

class Game
{
public:
	static void Initialize();

	static void SwitchChapter(char* chapter);
	static void ResetGame(int a1);

	static void PushScreen(int screenId, int a2);
	static int GetTopScreenID();
	static void PushOkDialog(const char* text, void(__cdecl* fn)(), int a3, int a4);
	static void PopScreen();

	static void InstancePost(int instance, int a2, int data);
	static int InstanceQuery(int instance, int a2);
	static int InstanceFind(int intro);

	static bool CheckChapter(char* chapter);
	static void SwitchPlayerCharacter();
	static DWORD AnimDataSomething(int a1, int a2, int a3);
	static void InstanceSetEventAnimPlaying(DWORD instance, int a2);

	static void IncreaseHealth(float amount);
	static void TriggerUiFadeGroup(int group);
	static void SetGameValue(int key, float val, bool apply);

	static bool m_binoculars;
	static void ToggleBinoculars();
private:
	static std::function<char(char* chapter)> f_SwitchChapter;
	static std::function<char __cdecl(int a1)> f_ResetGame;

	static std::function<char __cdecl(int screenId, int a2)> f_PushScreen;
	static std::function<int __cdecl()> f_GetTopScreenID;
	static std::function<int __cdecl(const char* text, int a2, int a3, int a4)> f_PushOkDialog;
	static std::function<int __cdecl()> f_PopScreen;
};