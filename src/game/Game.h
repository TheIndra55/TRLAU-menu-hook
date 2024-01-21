#pragma once

#include "instance/Instance.h"
#include "level/Stream.h"

struct menu_t;
struct MemCardInfo;
struct Level;
struct VertexPool;

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
	int field_30;
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
};
#endif

class Game
{
public:
	static Instance* GetPlayerInstance();
	static GameTracker* GetGameTracker();
	static STracker* GetStreamTracker();
};

void GAMELOOP_RequestLevelChangeByName(char* name, GameTracker* gameTracker, int doneType);