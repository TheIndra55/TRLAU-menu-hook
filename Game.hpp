#pragma once

#include <Windows.h>
#include <functional>

#if TRAE
	#define GAMETRACKER 0x838330
	#define GAMETRACKER_BASE_AREA (GAMETRACKER + 204)
	#define MAX_UNIT_LEN 32
	#define PLAYERINSTANCE 0x83833C

	#define DISKFS 0x838890
	#define CONFIGNAME "TRAE"
#elif TR8
	#define GAMETRACKER 0
	#define GAMETRACKER_BASE_AREA 0x00E7F0C0 // TODO find gametracker address
	#define MAX_UNIT_LEN 128
	#define PLAYERINSTANCE 0xE7F094

	#define DISKFS 0x9CE27C
	#define CONFIGNAME "TR8"
#elif TR7
	#define GAMETRACKER 0x010EE730
	#define GAMETRACKER_BASE_AREA (GAMETRACKER + 204)

	#define MAX_UNIT_LEN 32
	#define PLAYERINSTANCE (GAMETRACKER + 12)

	#define DISKFS 0x010EEC80
	#define CONFIGNAME "TR7"
#else
	#error "No game specified, set TRAE for Anniversary, TR7 for Legend or TR8 for Underworld"
#endif

namespace cdc
{
	struct Vector
	{
		float x, y, z, w;
	};
}

struct ObjectTracker
{
	__int32 resolveObj;
	char* objectName;
	DWORD* object;
	__int16 id;
	__int16 status;
};

struct Instance
{
	__int64 node;
	__int32 next;
	__int32 prev;
	cdc::Vector position;
	cdc::Vector oldPosition;
	cdc::Vector rotation;
	cdc::Vector oldRotation;
	cdc::Vector scale;
	cdc::Vector shadowPosition;
	cdc::Vector centerOfMass;
	__int32 matrix;
	__int32 oldMatrix;
	__int16 unk7;
	__int8 unk8;
	__int8 unk9;
	__int16 introNum;
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

	static void InstancePost(Instance* instance, int a2, int data);
	static int InstanceQuery(Instance* instance, int a2);
	static Instance* InstanceFind(int intro);

	static bool CheckChapter(char* chapter);
	static void SwitchPlayerCharacter();
	static DWORD AnimDataSomething(int a1, int a2, int a3);
	static void InstanceSetEventAnimPlaying(Instance* instance, int a2);

	static void IncreaseHealth(float amount);
	static void TriggerUiFadeGroup(int group);
	static void SetGameValue(int key, float val, bool apply);

	static bool m_binoculars;
	static void ToggleBinoculars();

	static void PlayerTurnGold();
	static void HideUnhideDrawGroup(int instance, int drawGroup, int on);
	static Instance* BirthObjectNoParent(int unitId, cdc::Vector* position, cdc::Vector* rotation, DWORD* introData, DWORD* object, int modelnum, int initEffects);

	static int GetObjectID(char* name);
};

class Stream
{
public:
	static ObjectTracker* GetObjectTrackerByName(char* name);
	static bool PollLoadQueue();
};