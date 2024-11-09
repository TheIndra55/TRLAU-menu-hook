#pragma once

#include "instance/Instance.h"
#include "level/Stream.h"

struct menu_t;
struct MemCardInfo;
struct Level;
struct VertexPool;

enum MainState
{
#ifndef TR8
	MS_NONE,
	MS_PLAYGAME,
	MS_LOADLEVEL,
	MS_PLAY_CINEMATIC,
	MS_SAMPLER_DEMO_DONE,
	MS_ATTRACT_MODE,
	MS_DISPLAY_MAIN_MENU,
	MS_SHOW_STATIC_SCREEN,
	MS_QUITGAME,
	MS_PLAY_DARKCHRONICLE_CINEMATIC,
	MS_PLAY_DARKCHRONICLE_CINEMATICMOVIE
#else
	MS_NONE,
	MS_PLAYGAME,
	MS_LOADLEVEL,
	MS_PLAY_CINEMATIC = 5,
	MS_SHOW_STATIC_SCREEN = 6,
	MS_DISPLAY_MAIN_MENU = 10
#endif
};

struct WipeInfo
{
	float wipeCur;
	float wipeTarget;
	float wipeStep;
};

#ifndef TR8
struct GameTracker
{
	menu_t* menu;
	MemCardInfo* memcard;
	Level* level;
	Instance* playerInstance;
	VertexPool* vertexPool;

	int debugFlags;
	int debugFlags2;
	int debugFlags3;
	int debugFlags4;

	WipeInfo wipes[12];

	unsigned int displayFrameCount;
	unsigned int frameCount;

	int fpsFast;
	int gameFlags;
	int streamFlags;
	Level* mainDrawUnit;

	char baseAreaName[20];

	char levelDone;
	char levelChange;
	char loadingDisplayType;
	char gameMode;
	char cheatMode;
	char savingGame;
	__int16 postSaveScreenID;

	int StreamUnitID;

	char pad1[60];

	float timeMult;
	float unmodifiedActualTimeMult;
	float actualTimeMult;
	float globalTimeMult;

	float scrollProgress;
	float timeDilation;

	int debugTimeMult;

	char pad2[752];

	int currentPlayerObjectID;
	int altPlayerObjectID;
};
#else
struct GameTracker
{
	int field_0;
	int field_4;
	Level* level;
	Instance* playerInstance;

	int debugFlags;
	int debugFlags2;
	int debugFlags3;
	int debugFlags4;

	int displayFrameCount;
	int field_24;
	int field_28;
	int field_2C;
	int streamFlags;
	int field_34;

	char baseAreaName[128];
	char field_B8;
	char field_B9;
	char field_BA;

	char gameMode;
	char cheatMode;

	char field_BD;
	char field_BE;
	char field_BF;
	int StreamUnitID;
	int field_C4;
	int field_C8;
	int field_CC;
	int field_D0;
	int field_D4;
	int field_D8;
	int field_DC;
	int field_E0;
	int field_E4;
	int field_E8;
	int field_EC;
	int field_F0;
	int field_F4;
	int field_F8;
	int field_FC;

	float timeMult;

	int field_104;
	int field_108;
	int field_10C;
	int field_110;
	int field_114;
	int field_118;
	int field_11C;
	int field_120;
	int field_124;
	int field_128;
	int field_12C;
	int field_130;
	int field_134;
	int field_138;
	int field_13C;
	int currentPlayerObjectID;
	int altPlayerObjectID;

	float timeDilation;
};

struct PlayerObjects
{
	__int16 numPlayerObjects;
	__int16* playerObjectList;
};

struct GlobalInfo
{
	int field_0;
	PlayerObjects* playerObjects;
};
#endif

class Game
{
public:
	static Instance* GetPlayerInstance() noexcept;
	static GameTracker* GetGameTracker() noexcept;
	static STracker* GetStreamTracker() noexcept;

	static void SwitchPlayerCharacter() noexcept;

	static bool IsInNextGenMode() noexcept;
};

void GAMELOOP_RequestLevelChangeByName(char* name, GameTracker* gameTracker, int doneType);
bool GAMELOOP_IsWipeDone(int type);
void GAMELOOP_SetScreenWipe(int type, int target, int time);

void PLAYER_DebugSwitchPlayerCharacter();

int OBTABLE_GetObjectID(char* name);
char* OBTABLE_GetObjectName(int id);

void LOAD_ObjectFileName(char* name, char* object, char* extension);
void LOAD_UnitFileName(char* name, char* unit, char* extension);