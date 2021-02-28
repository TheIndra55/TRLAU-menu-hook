#pragma once
#include <functional>
#include "pch.h"

#define GAMETRACKER 0x838330

#if TRAE
	#define GAMETRACKER_BASE_AREA (GAMETRACKER + 204)
	#define MAX_UNIT_LEN 32
	#define PLAYERINSTANCE 0x83833C
#elif TR8
	#define GAMETRACKER_BASE_AREA 0x00E7F0C0 // TODO find gametracker address
	#define MAX_UNIT_LEN 128
	#define PLAYERINSTANCE 0xE7F094
#else
	#error "No game specified, set TRAE for Anniversary or TR8 for Underworld"
#endif

// recycled from another unfinished project of mine, needs more research
namespace cdc
{
	struct Vector3
	{
		float x, y, z;
		float unused;
	};
}

struct ObjectTracker
{
	__int32 unk1;
	__int32 unk2;
	DWORD* object;
	__int16 id;
	__int16 status;
};

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

struct Level
{
};

struct StreamUnit
{
	int id;
	__int8 used;
	__int8 unitHidden;
	__int16 flags;
	Level* level;
};

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
	static void PlayerTurnGold();
	static void HideUnhideDrawGroup(int instance, int drawGroup, int on);
	static int BirthObjectNoParent(int unitId, cdc::Vector3* position, cdc::Vector3* rotation, DWORD* introData, DWORD* object, int modelnum, int initEffects);
private:
	static std::function<char(char* chapter)> f_SwitchChapter;

	static std::function<char __cdecl(int screenId, int a2)> f_PushScreen;
	static std::function<int __cdecl()> f_GetTopScreenID;
	static std::function<int __cdecl(const char* text, int a2, int a3, int a4)> f_PushOkDialog;
	static std::function<int __cdecl()> f_PopScreen;
};

class Stream
{
public:
	static ObjectTracker* GetObjectTrackerByName(char* name);
	static bool PollLoadQueue();
};