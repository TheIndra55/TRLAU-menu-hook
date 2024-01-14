#pragma once

#include "instance/Instance.h"

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

	char pad1[66];

	float timeMult;
};

class Game
{
public:
	static void Init();

	static Instance* GetPlayerInstance();
	static GameTracker* GetGameTracker();
};