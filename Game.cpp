#include "include/MinHook.h"
#include "Game.hpp"
#include "Hooking.hpp"

bool Game::m_binoculars = false;

void(__cdecl* GAMELOOP_ExitGame)(int a1);
void(__cdecl* CHRONICLE_SwitchChapter)(char* chapter);
char(__cdecl* UIScreenManager_PushScreen)(int screenId, int a2);
int(__cdecl* UIScreenManager_GetTopScreenID)();
int(__cdecl* UIScreenManager_PushOkDialog)(const char* text, int a2, int a3, int a4);
int(__cdecl* UIScreenManager_PopScreen)();

#if TRAE || TR7
void(__cdecl* PLAYER_DebugSwitchPlayerCharacter)();
#elif TR8
// has an extra arg in Underworld
int(__cdecl* PLAYER_DebugSwitchPlayerCharacter)(DWORD a1);
#endif

DWORD(__cdecl* sub_C64D3F)(int a1, int a2, int a3);
void(__cdecl* G2EmulationInstanceSetEventAnimPlaying)(Instance* instance, int a2);

void(__cdecl* INSTANCE_Post)(Instance*, DWORD, int);
int(__cdecl* INSTANCE_Query)(Instance*, int);
Instance*(__cdecl* INSTANCE_Find)(int);

void(__cdecl* IncrHealth)(float amount);
void(__cdecl* UIFadeGroupTrigger)(int group);
void(__cdecl* game_SetGameValue)(int a1, float a2, char a3);
double(__cdecl* game_GetGameValue)(int a1);

void(__cdecl* PLAYER_SetLookAround)(Instance* instance);
void(__cdecl* CAMERA_StartLookaroundMode)(DWORD camera);
int(__cdecl* SteerSwitchMode)(Instance* instance, int mode);
void(__cdecl* PLAYER_ReSetLookAround)(Instance* instance);
__int16(__cdecl* CAMERA_ForceEndLookaroundMode)(DWORD camera);

void(__cdecl* sub_456B50)();
void(__cdecl* EVENT_PlayerTurnGold)();
void(__cdecl* INSTANCE_HideUnhideDrawGroup)(int a1, int a2, int a3);

int(__cdecl* OBTABLE_GetObjectID)(char* name);

char* (__cdecl* game_localstr_get)(int a1);

Instance* (__cdecl* INSTANCE_BirthObjectNoParent)(int unitId, cdc::Vector* position, cdc::Vector* rotation, DWORD* introData, DWORD* object, int modelnum, int);
ObjectTracker*(__cdecl* STREAM_GetObjectTrackerByName)(char*);
bool(__cdecl* STREAM_PollLoadQueue)();

bool(__cdecl* GAMELOOP_IsWipeDone)(int type);
void(__cdecl* GAMELOOP_SetScreenWipe)(int type, int target, int time);

// deathstate stuff
int(__stdcall* DeathState_Entry)(int a1, int a2);
void(__stdcall* DeathState_Process)(int a1, int a2);

int __stdcall DeathStateEntry(int a1, int a2)
{
	auto ret = DeathState_Entry(a1, a2);

	if (!Hooking::GetInstance().GetMenu()->m_drawSettings.noRespawn)
	{
		// we NOPed the orginal screenwipe code so call it manually
		if (GAMELOOP_IsWipeDone(10))
		{
			GAMELOOP_SetScreenWipe(10, 100, 90);
		}
	}

	return ret;
}

void __stdcall DeathStateProcess(int a1, int a2)
{
	if (!Hooking::GetInstance().GetMenu()->m_drawSettings.noRespawn)
	{
		DeathState_Process(a1, a2);
	}
}

bool(__cdecl* UIScreenManager_WantsNormalFading)();

bool HookedWipe11_WantsNormalFading()
{
	if (Hooking::GetInstance().GetMenu()->m_drawSettings.noMovieBars)
	{
		return false;
	}

	return UIScreenManager_WantsNormalFading();
}

void Game::Initialize()
{
	CHRONICLE_SwitchChapter = reinterpret_cast<void(__cdecl*)(char*)>(0x422090);

#if TRAE
	GAMELOOP_ExitGame = reinterpret_cast<void(__cdecl*)(int)>(0x4542B0);

	GAMELOOP_SetScreenWipe = reinterpret_cast<void(__cdecl*)(int type, int target, int time)>(0x00452A30);
	GAMELOOP_IsWipeDone = reinterpret_cast<bool(__cdecl*)(int type)>(0x00452970);
#elif TR8
	GAMELOOP_ExitGame = reinterpret_cast<void(__cdecl*)(int)>(0x5DF760);

	GAMELOOP_SetScreenWipe = reinterpret_cast<void(__cdecl*)(int type, int target, int time)>(0x0052E8B0);
	GAMELOOP_IsWipeDone = reinterpret_cast<bool(__cdecl*)(int type)>(0x0052E880);
#elif TR7
	GAMELOOP_ExitGame = reinterpret_cast<void(__cdecl*)(int)>(0x454550);

	GAMELOOP_SetScreenWipe = reinterpret_cast<void(__cdecl*)(int type, int target, int time)>(0x00452F40);
	GAMELOOP_IsWipeDone = reinterpret_cast<bool(__cdecl*)(int type)>(0x00452E80);
#endif

	UIScreenManager_PushScreen = reinterpret_cast<char(__cdecl*)(int, int)>(0x4FCB60);
	UIScreenManager_GetTopScreenID = reinterpret_cast<int(__cdecl*)()>(0x4FC210);
	UIScreenManager_PushOkDialog = reinterpret_cast<int(__cdecl*)(const char*, int, int, int)>(0x4FD100);
	UIScreenManager_PopScreen = reinterpret_cast<int(__cdecl*)()>(0x4FCD20);

#if TRAE
	INSTANCE_Post = reinterpret_cast<void(__cdecl*)(Instance*, DWORD, int)>(0x4580B0);
#elif TR7
	INSTANCE_Post = reinterpret_cast<void(__cdecl*)(Instance*, DWORD, int)>(0x458250);
#elif TR8
	INSTANCE_Post = reinterpret_cast<void(__cdecl*)(Instance*, DWORD, int)>(0x5B3750);
#endif

#if TRAE
	INSTANCE_Query = reinterpret_cast<int(__cdecl*)(Instance*, int)>(0x00458060);
#elif TR7
	INSTANCE_Query = reinterpret_cast<int(__cdecl*)(Instance*, int)>(0x00458200);
#endif

	INSTANCE_Find = reinterpret_cast<Instance*(__cdecl*)(int)>(0x004582D0);

#if TRAE
	PLAYER_DebugSwitchPlayerCharacter = reinterpret_cast<void(__cdecl*)()>(0x005A39A0);
#elif TR8
	PLAYER_DebugSwitchPlayerCharacter = reinterpret_cast<int(__cdecl*)(DWORD)>(0x0079DB50);
#elif TR7
	PLAYER_DebugSwitchPlayerCharacter = reinterpret_cast<void(__cdecl*)()>(0x005A9A90);
#endif
	G2EmulationInstanceSetEventAnimPlaying = reinterpret_cast<void(__cdecl*)(Instance*, int)>(0x4DE940);

	IncrHealth = reinterpret_cast<void(__cdecl*)(float)>(0x005715E0);
	UIFadeGroupTrigger = reinterpret_cast<void(__cdecl*)(int)>(0x004EE580);
	game_SetGameValue = reinterpret_cast<void(__cdecl*)(int, float, char)>(0x004551A0);
	game_GetGameValue = reinterpret_cast<double(__cdecl*)(int)>(0x004551E0);

#if TRAE
	MH_CreateHook((void*)0x4E3C80, localstr_get, (void**)&game_localstr_get);

	NOP((void*)0x005584DC, 5);
	MH_CreateHook((void*)0x005699C0, DeathStateProcess, (void**)&DeathState_Process);
	MH_CreateHook((void*)0x005581D0, DeathStateEntry, (void**)&DeathState_Entry);

	UIScreenManager_WantsNormalFading = reinterpret_cast<bool(__cdecl*)()>(0x004FC1F0);

	auto call = int{ (int)HookedWipe11_WantsNormalFading - 0x00452F92 - 4};
	memcpy((void*)0x00452F92, &call, sizeof(call));

	//MH_CreateHook((void*)0x004FC1F0, UIScreenManager_WantsNormalFading, nullptr);
#elif TR8
	NOP((void*)0x0075AEDE, 5);
	MH_CreateHook((void*)0x0075AF90, DeathStateProcess, (void**)&DeathState_Process);
	MH_CreateHook((void*)0x0075AA50, DeathStateEntry, (void**)&DeathState_Entry);
#elif TR7
	MH_CreateHook((void*)0x4E7690, localstr_get, (void**)&game_localstr_get);

	NOP((void*)0x00563478, 5);
	MH_CreateHook((void*)0x00574320, DeathStateProcess, (void**)&DeathState_Process);
	MH_CreateHook((void*)0x005631B0, DeathStateEntry, (void**)&DeathState_Entry);

	UIScreenManager_WantsNormalFading = reinterpret_cast<bool(__cdecl*)()>(0x004FC130);

	// .text section in nextgen debug exe is read only
	DWORD lpflOldProtect, _;
	VirtualProtect((void*)0x453499, 4, PAGE_EXECUTE_READWRITE, &lpflOldProtect);

	auto call = int{ (int)HookedWipe11_WantsNormalFading - 0x453499 - 4 };
	memcpy((void*)0x453499, &call, sizeof(call));

	VirtualProtect((void*)0x453499, 4, lpflOldProtect, &_);
#endif

	PLAYER_SetLookAround = reinterpret_cast<void(__cdecl*)(Instance*)>(0x005600F0);
	CAMERA_StartLookaroundMode = reinterpret_cast<void(__cdecl*)(DWORD)>(0x0048A300);
	SteerSwitchMode = reinterpret_cast<int(__cdecl*)(Instance*, int)>(0x005BAE60);
	PLAYER_ReSetLookAround = reinterpret_cast<void(__cdecl*)(Instance * instance)>(0x00560110);
	CAMERA_ForceEndLookaroundMode = reinterpret_cast<__int16(__cdecl*)(DWORD)>(0x0048A5E0);

	sub_456B50 = reinterpret_cast<void(__cdecl*)()>(0x456B50);
	EVENT_PlayerTurnGold = reinterpret_cast<void(__cdecl*)()>(0x0044E290);
	INSTANCE_HideUnhideDrawGroup = reinterpret_cast<void(__cdecl*)(int, int, int)>(0x004319B0);

#if TRAE
	OBTABLE_GetObjectID = reinterpret_cast<int(__cdecl*)(char*)>(0x4660C0);

	INSTANCE_BirthObjectNoParent = reinterpret_cast<Instance*(__cdecl*)(int, cdc::Vector*, cdc::Vector*, DWORD*, DWORD*, int, int)>(0x0045BA90);
	STREAM_GetObjectTrackerByName = reinterpret_cast<ObjectTracker*(__cdecl*)(char*)>(0x5DA260);
	STREAM_PollLoadQueue = reinterpret_cast<bool(__cdecl*)()>(0x005DB190);
#elif TR7
	OBTABLE_GetObjectID = reinterpret_cast<int(__cdecl*)(char*)>(0x004655B0);

	INSTANCE_BirthObjectNoParent = reinterpret_cast<Instance * (__cdecl*)(int, cdc::Vector*, cdc::Vector*, DWORD*, DWORD*, int, int)>(0x0045B710);
	STREAM_GetObjectTrackerByName = reinterpret_cast<ObjectTracker * (__cdecl*)(char*)>(0x005DA400);
	STREAM_PollLoadQueue = reinterpret_cast<bool(__cdecl*)()>(0x005DB350);
#elif TR8
	OBTABLE_GetObjectID = reinterpret_cast<int(__cdecl*)(char*)>(0x005BF770);

	INSTANCE_BirthObjectNoParent = reinterpret_cast<Instance * (__cdecl*)(int, cdc::Vector*, cdc::Vector*, DWORD*, DWORD*, int, int)>(0x005BD0F0);
	STREAM_GetObjectTrackerByName = reinterpret_cast<ObjectTracker * (__cdecl*)(char*)>(0x005C17D0);
	STREAM_PollLoadQueue = reinterpret_cast<bool(__cdecl*)()>(0x005C1DA0);
#endif
}

char* localstr_get(int a1)
{
	return game_localstr_get(a1);
}

void Game::SwitchChapter(char* chapter)
{
	CHRONICLE_SwitchChapter(chapter);
}

void Game::ResetGame(int unk1)
{
	GAMELOOP_ExitGame(unk1);
}

void Game::PushScreen(int screenId, int unk2)
{
	UIScreenManager_PushScreen(screenId, unk2);
}

int Game::GetTopScreenID()
{
	return UIScreenManager_GetTopScreenID();
}

void Game::PushOkDialog(const char* text, void(__cdecl* fn)(), int a3, int a4)
{
	UIScreenManager_PushOkDialog(text, (int)fn, a3, a4);
}

void Game::PopScreen()
{
	UIScreenManager_PopScreen();
}

// Instance post, query and find are mostly used to interact with instances from a level e.g. doors, levers, traps
// for example to query a lever Game::InstanceQuery(leverInstance, 233) == 1 // lever is switched on
// more can be found here https://theindra.eu/projects/TRAE-Research/research.html#_example
void Game::InstancePost(Instance* instance, int a2, int data)
{
	return INSTANCE_Post(instance, a2, data);
}

int Game::InstanceQuery(Instance* instance, int a2)
{
	return INSTANCE_Query(instance, a2);
}

Instance* Game::InstanceFind(int intro)
{
	return INSTANCE_Find(intro);
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
#if TRAE || TR7
	PLAYER_DebugSwitchPlayerCharacter();
#elif TR8
	// patch numPlayerObjects to exclude missing DLC outfits
	auto ptr = *(int*)0xE7EE50;
	auto ptr2 = *(int*)(ptr + 4);
	auto numPlayerObjects = (__int16*)ptr2;
	*numPlayerObjects = 10;

	auto playerObjects = *(int*)(ptr2 + 4);
	*(__int16*)(playerObjects + 6) = 12; // patch the entire list to skip lara_doppelganger since that one is missing, dammit CD
	*(__int16*)(playerObjects + 8) = 13;
	*(__int16*)(playerObjects + 10) = 14;
	*(__int16*)(playerObjects + 12) = 16; // lara_mom_thrall also doesn't exist, skip too
	*(__int16*)(playerObjects + 14) = 17;
	*(__int16*)(playerObjects + 16) = 18;
	*(__int16*)(playerObjects + 18) = 19;

	int a1 = 12; // im not sure what this value is, passing 12 works
	PLAYER_DebugSwitchPlayerCharacter((DWORD)&a1);
#endif
}

void Game::InstanceSetEventAnimPlaying(Instance* instance, int a2)
{
	G2EmulationInstanceSetEventAnimPlaying(instance, a2);
}

void Game::IncreaseHealth(float amount)
{
	IncrHealth(amount);
}

void Game::TriggerUiFadeGroup(int group)
{
	UIFadeGroupTrigger(group);
}

void Game::SetGameValue(int key, float val, bool apply)
{
	game_SetGameValue(key, val, apply);
}

double Game::GetGameValue(int key)
{
	return game_GetGameValue(key);
}

void Game::ToggleBinoculars()
{
	auto player = (Instance*)PLAYERINSTANCE;

	if (!m_binoculars)
	{
		SteerSwitchMode(player, 0);

		*(__int16*)0x850CAC = 1;
		*(bool*)0x850418 = 1;
		*(int*)0x86C908 = 72;
		*(int*)0x86C818 = 3;

		PLAYER_SetLookAround(player);
		Game::InstancePost(player, 262265, 0);
		Game::InstancePost(player, 262264, 8);
		CAMERA_StartLookaroundMode(0x850670);
		sub_456B50();
	}
	else
	{
		*(int*)0x6926FC = 0;
		*(int*)0x86CD90 = 0;

		PLAYER_ReSetLookAround(player);
		Game::InstancePost(player, 262266, 0);
		CAMERA_ForceEndLookaroundMode(0x850670);
	}

	m_binoculars = !m_binoculars;
}

// Activates the gold wet effect, calling this outside unit gr18 crashes the game
void Game::PlayerTurnGold()
{
	EVENT_PlayerTurnGold();
}

void Game::HideUnhideDrawGroup(int instance, int drawGroup, int on)
{
	INSTANCE_HideUnhideDrawGroup(instance, drawGroup, on);
}

Instance* Game::BirthObjectNoParent(int unitId, cdc::Vector* position, cdc::Vector* rotation, DWORD* introData, DWORD* object, int modelnum, int initEffects)
{
	return INSTANCE_BirthObjectNoParent(unitId, position, rotation, introData, object, modelnum, initEffects);
}

ObjectTracker* Stream::GetObjectTrackerByName(char* name)
{
	return STREAM_GetObjectTrackerByName(name);
}

bool Stream::PollLoadQueue()
{
	return STREAM_PollLoadQueue();
}

int Game::GetObjectID(char* name)
{
	return OBTABLE_GetObjectID(name);
}