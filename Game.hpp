#pragma once
#include <functional>
#include "pch.h"

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

	static int InstancePost(int a1, int a2, int a3);

	static bool CheckChapter(char* chapter);
private:
	static std::function<char(char* chapter)> f_SwitchChapter;
	static std::function<char __cdecl(int a1)> f_ResetGame;

	static std::function<char __cdecl(int screenId, int a2)> f_PushScreen;
	static std::function<int __cdecl()> f_GetTopScreenID;
	static std::function<int __cdecl(const char* text, int a2, int a3, int a4)> f_PushOkDialog;
	static std::function<int __cdecl()> f_PopScreen;

	static std::function<int __cdecl(int, int, int)> f_InstancePost;
};