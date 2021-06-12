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
	#define MAINFONT 0x007D1800
	#define GCHEATWIREFRAME 0x7C7CD4
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
	#define MAINFONT 0x01088A2C
	#define GCHEATWIREFRAME 0x107E580
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

#if TRAE || TR7
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
#elif TR8
struct Instance
{
	__int64 node;
	Instance* next;
	__int32 prev;
	DWORD* object;
	char padding[12];
	cdc::Vector position;
	cdc::Vector rotation;
};
#endif

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

struct Model
{
	int version;
	int numSegments;
	int numVirtSegments;
	int segmentList;
	cdc::Vector modelScale;
	int numVertices;
	int vertexList;
	int numNormals;
	int normalList;
	int numFaces;
	int faceList;
};

struct DRAWVERTEX
{
	float x;
	float y;
	float z;
	int color;
	float u;
	float v;
	__int64 anPad;
};

// taken from TRAE.pdb
// should be compatible with TR7
enum MUD_FLAGS
{
	MUD_FLAG_NONE = 0,
	MUD_FLAG_POLYLINE = 1,
	MUD_FLAG_INSTANCE = 2,
	MUD_FLAG_WRAPS = 4,
	MUD_FLAG_PATTERN_INTERACT = 0x2000,
	MUD_FLAG_ENEMY_JUMP_ACROSS = 0x4000,
	MUD_FLAG_ENEMY_JUMP_DOWN = 0x8000,
	MUD_FLAG_ENEMY_JUMP_UP = 0x10000,
	MUD_FLAG_ATTACHPOINTS = 0x20000,
	MUD_FLAG_PERCH = 0x40000,
	MUD_FLAG_ROPE = 0x80000,
	MUD_FLAG_WATER = 0x100000,
	MUD_FLAG_CLOSEJUMPTARGET = 0x200000,
	MUD_FLAG_LADDER = 0x400000,
	MUD_FLAG_MANTLEDOWN = 0x800000,
	MUD_FLAG_ZIPLINE = 0x1000000,
	MUD_FLAG_HORIZPOLE = 0x2000000,
	MUD_FLAG_VERTPOLE = 0x4000000,
	MUD_FLAG_WALLVERTPOLE = 0x8000000,
	MUD_FLAG_ENEMYDROPDOWN = 0x10000000,
	MUD_FLAG_JUMPLANDING = 0x20000000,
	MUD_FLAG_LEDGEGRAB = 0x40000000,
	MUD_FLAG_DISABLED = 0x80000000,
	MUD_FLAG_ALL_TYPES = 0x0FFFDC000,
	MUD_FLAG_ALL = 0x0FFFFFFFF
};

struct MudFlag
{
	int flag;
	const char* name;
};

constexpr MudFlag mudFlags[22]
{
	MudFlag { MUD_FLAG_NONE, "NONE | " },
	MudFlag { MUD_FLAG_INSTANCE, "INSTANCE | " },
	MudFlag { MUD_FLAG_WRAPS, "WRAPS | " },
	MudFlag { MUD_FLAG_PATTERN_INTERACT, "PATTERN_INTERACT | " },
	MudFlag { MUD_FLAG_ENEMY_JUMP_ACROSS, "ENEMY_JUMP_ACROSS | " },
	MudFlag { MUD_FLAG_ENEMY_JUMP_DOWN, "ENEMY_JUMP_DOWN | " },
	MudFlag { MUD_FLAG_ENEMY_JUMP_UP, "ENEMY_JUMP_UP | " },
	MudFlag { MUD_FLAG_ATTACHPOINTS, "ATTACHPOINTS | " },
	MudFlag { MUD_FLAG_PERCH, "PERCH | " },
	MudFlag { MUD_FLAG_ROPE, "ROPE | " },
	MudFlag { MUD_FLAG_WATER, "WATER | " },
	MudFlag { MUD_FLAG_CLOSEJUMPTARGET, "CLOSEJUMPTARGET | " },
	MudFlag { MUD_FLAG_LADDER, "LADDER | " },
	MudFlag { MUD_FLAG_MANTLEDOWN, "MANTLEDOWN | " },
	MudFlag { MUD_FLAG_ZIPLINE, "ZIPLINE | " },
	MudFlag { MUD_FLAG_HORIZPOLE, "HORIZPOLE | " },
	MudFlag { MUD_FLAG_VERTPOLE, "VERTPOLE | " },
	MudFlag { MUD_FLAG_WALLVERTPOLE, "WALLVERTPOLE | " },
	MudFlag { MUD_FLAG_ENEMYDROPDOWN, "ENEMYDROPDOWN | " },
	MudFlag { MUD_FLAG_JUMPLANDING, "JUMPLANDING | " },
	MudFlag { MUD_FLAG_LEDGEGRAB, "LEDGEGRAB | " },
	MudFlag { MUD_FLAG_DISABLED, "DISABLED | " },
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