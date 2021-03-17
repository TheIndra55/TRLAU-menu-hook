#include "Game.hpp"

bool Game::m_binoculars = false;

void(__cdecl* GAMELOOP_ExitGame)(int a1);
void(__cdecl* CHRONICLE_SwitchChapter)(char* chapter);
char(__cdecl* UIScreenManager_PushScreen)(int screenId, int a2);
int(__cdecl* UIScreenManager_GetTopScreenID)();
int(__cdecl* UIScreenManager_PushOkDialog)(const char* text, int a2, int a3, int a4);
int(__cdecl* UIScreenManager_PopScreen)();

#if TRAE
void(__cdecl* PLAYER_DebugSwitchPlayerCharacter)();
#elif TR8
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

void(__cdecl* PLAYER_SetLookAround)(Instance* instance);
void(__cdecl* CAMERA_StartLookaroundMode)(DWORD camera);
int(__cdecl* SteerSwitchMode)(Instance* instance, int mode);
void(__cdecl* PLAYER_ReSetLookAround)(Instance* instance);
__int16(__cdecl* CAMERA_ForceEndLookaroundMode)(DWORD camera);

void(__cdecl* sub_456B50)();
void(__cdecl* EVENT_PlayerTurnGold)();
void(__cdecl* INSTANCE_HideUnhideDrawGroup)(int a1, int a2, int a3);

Instance* (__cdecl* INSTANCE_BirthObjectNoParent)(int unitId, cdc::Vector* position, cdc::Vector* rotation, DWORD* introData, DWORD* object, int modelnum, int);
ObjectTracker*(__cdecl* STREAM_GetObjectTrackerByName)(char*);
bool(__cdecl* STREAM_PollLoadQueue)();

void Game::Initialize()
{
	CHRONICLE_SwitchChapter = reinterpret_cast<void(__cdecl*)(char*)>(0x422090);

#if TRAE
	GAMELOOP_ExitGame = reinterpret_cast<void(__cdecl*)(int)>(0x4542B0);
#elif TR8
	GAMELOOP_ExitGame = reinterpret_cast<void(__cdecl*)(int)>(0x5DF760);
#endif

	UIScreenManager_PushScreen = reinterpret_cast<char(__cdecl*)(int, int)>(0x4FCB60);
	UIScreenManager_GetTopScreenID = reinterpret_cast<int(__cdecl*)()>(0x4FC210);
	UIScreenManager_PushOkDialog = reinterpret_cast<int(__cdecl*)(const char*, int, int, int)>(0x4FD100);
	UIScreenManager_PopScreen = reinterpret_cast<int(__cdecl*)()>(0x4FCD20);

#if TRAE
	INSTANCE_Post = reinterpret_cast<void(__cdecl*)(Instance*, DWORD, int)>(0x004580B0);
#elif TR8
	INSTANCE_Post = reinterpret_cast<void(__cdecl*)(Instance*, DWORD, int)>(0x69A670);
#endif

	INSTANCE_Query = reinterpret_cast<int(__cdecl*)(Instance*, int)>(0x00458060);
	INSTANCE_Find = reinterpret_cast<Instance*(__cdecl*)(int)>(0x004582D0);

#if TRAE
	PLAYER_DebugSwitchPlayerCharacter = reinterpret_cast<void(__cdecl*)()>(0x005A39A0);
#elif TR8
	PLAYER_DebugSwitchPlayerCharacter = reinterpret_cast<int(__cdecl*)(DWORD)>(0x0079DB50);
#endif
	sub_C64D3F = reinterpret_cast<DWORD(__cdecl*)(int, int, int)>(0xC64D3F);
	G2EmulationInstanceSetEventAnimPlaying = reinterpret_cast<void(__cdecl*)(Instance*, int)>(0x4DE940);

	IncrHealth = reinterpret_cast<void(__cdecl*)(float)>(0x005715E0);
	UIFadeGroupTrigger = reinterpret_cast<void(__cdecl*)(int)>(0x004EE580);
	game_SetGameValue = reinterpret_cast<void(__cdecl*)(int, float, char)>(0x004551A0);

	PLAYER_SetLookAround = reinterpret_cast<void(__cdecl*)(Instance*)>(0x00C759A8);
	CAMERA_StartLookaroundMode = reinterpret_cast<void(__cdecl*)(DWORD)>(0x0048A300);
	SteerSwitchMode = reinterpret_cast<int(__cdecl*)(Instance*, int)>(0x005BAE60);
	PLAYER_ReSetLookAround = reinterpret_cast<void(__cdecl*)(Instance * instance)>(0x00C759C7);
	CAMERA_ForceEndLookaroundMode = reinterpret_cast<__int16(__cdecl*)(DWORD)>(0x0048A5E0);

	sub_456B50 = reinterpret_cast<void(__cdecl*)()>(0x456B50);
	EVENT_PlayerTurnGold = reinterpret_cast<void(__cdecl*)()>(0x0044E290);
	INSTANCE_HideUnhideDrawGroup = reinterpret_cast<void(__cdecl*)(int, int, int)>(0x004319B0);

	INSTANCE_BirthObjectNoParent = reinterpret_cast<Instance*(__cdecl*)(int, cdc::Vector*, cdc::Vector*, DWORD*, DWORD*, int, int)>(0x0045BA90);
	STREAM_GetObjectTrackerByName = reinterpret_cast<ObjectTracker*(__cdecl*)(char*)>(0x00C7D67C);
	STREAM_PollLoadQueue = reinterpret_cast<bool(__cdecl*)()>(0x005DB190);
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
#if TRAE
	PLAYER_DebugSwitchPlayerCharacter();
#elif TR8
	// patch numPlayerObjects to exclude missing DLC outfits
	auto ptr = *(int*)0xE7EE50;
	auto ptr2 = *(int*)(ptr + 4);
	auto numPlayerObjects = (__int16*)ptr2;
	*numPlayerObjects = 10;

	auto playerObjects = *(int*)(ptr2 + 4);
	*(__int16*)(playerObjects + 6) = 12; // patch the entire list to skip lara_doppelganger since that one is missing, dammit square enix
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

DWORD Game::AnimDataSomething(int a1, int a2, int a3)
{
	return sub_C64D3F(a1, a2, a3);
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