#include "Game.hpp"

// todo ouch
std::function<char(char*)> Game::f_SwitchChapter = nullptr;
std::function<char __cdecl(int)> Game::f_ResetGame = nullptr;
std::function<char __cdecl(int, int)> Game::f_PushScreen = nullptr;
std::function<int __cdecl()> Game::f_GetTopScreenID = nullptr;
std::function<int __cdecl(const char*, int, int, int)> Game::f_PushOkDialog = nullptr;
std::function<int __cdecl()> Game::f_PopScreen = nullptr;
std::function<int __cdecl(int, int, int)> Game::f_InstancePost = nullptr;

void(__cdecl* PLAYER_DebugSwitchPlayerCharacter)();
DWORD(__cdecl* sub_C64D3F)(int a1, int a2, int a3);
void(__cdecl* G2EmulationInstanceSetEventAnimPlaying)(DWORD instance, int a2);

void Game::Initialize()
{
	f_SwitchChapter = reinterpret_cast<char(__cdecl*)(char*)>(0x422090);
	f_ResetGame = reinterpret_cast<char(__cdecl*)(int)>(0x4542B0);

	f_PushScreen = reinterpret_cast<char(__cdecl*)(int, int)>(0x4FCB60);
	f_GetTopScreenID = reinterpret_cast<int(__cdecl*)()>(0x4FC210);
	f_PushOkDialog = reinterpret_cast<int(__cdecl*)(const char*, int, int, int)>(0x4FD100);
	f_PopScreen = reinterpret_cast<int(__cdecl*)()>(0x4FCD20);

	f_InstancePost = reinterpret_cast<int(__cdecl*)(int, int, int)>(0x4580B0);

	PLAYER_DebugSwitchPlayerCharacter = reinterpret_cast<void(__cdecl*)()>(0x005A39A0);
	sub_C64D3F = reinterpret_cast<DWORD(__cdecl*)(int, int, int)>(0xC64D3F);
	G2EmulationInstanceSetEventAnimPlaying = reinterpret_cast<void(__cdecl*)(DWORD, int)>(0x4DE940);
}

void Game::SwitchChapter(char* chapter)
{
	f_SwitchChapter(chapter);
}

void Game::ResetGame(int unk1)
{
	f_ResetGame(unk1);
}

void Game::PushScreen(int screenId, int unk2)
{
	f_PushScreen(screenId, unk2);
}

int Game::GetTopScreenID()
{
	return f_GetTopScreenID();
}

void Game::PushOkDialog(const char* text, void(__cdecl* fn)(), int a3, int a4)
{
	f_PushOkDialog(text, (int)fn, a3, a4);
}

void Game::PopScreen()
{
	f_PopScreen();
}

int Game::InstancePost(int instance, int a2, int a3)
{
	return f_InstancePost(instance, a2, a3);
}

bool Game::CheckChapter(char* chapter)
{
	auto base = *reinterpret_cast<DWORD*>(0x812C68);
	int chapters = *reinterpret_cast<int*>(base + 24);

	int i = 0;
	int offset = 0;
	while (std::strcmp(chapter, 
		reinterpret_cast<char*>(
			*reinterpret_cast<DWORD*>(base + 28) + offset)))
	{
		i++;
		offset += 400;

		if (i >= chapters)
		{
			return false;
		}
	}

	return true;
}

void Game::SwitchPlayerCharacter()
{
	PLAYER_DebugSwitchPlayerCharacter();
}

DWORD Game::AnimDataSomething(int a1, int a2, int a3)
{
	return sub_C64D3F(a1, a2, a3);
}

void Game::InstanceSetEventAnimPlaying(DWORD instance, int a2)
{
	G2EmulationInstanceSetEventAnimPlaying(instance, a2);
}