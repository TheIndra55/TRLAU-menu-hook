#pragma once

#include "instance/Object.h"
#include "level/Level.h"

#if TRAE || TR7
#define MAX_UNIT_NAME_LENGTH 20
#else
#define MAX_UNIT_NAME_LENGTH 128
#endif

#define MAX_STREAM_UNITS 8

struct ResolveObject;

struct StreamUnit
{
	int StreamUnitID;

	char used;
	char unitHidden;
	__int16 unitFlags;

	Level* level;
	char baseAreaName[20];

	ResolveObject* resolveObj;

	char pad1[56];
};

struct STracker
{
	StreamUnit StreamList[MAX_STREAM_UNITS];
};

struct ObjectTracker
{
	ResolveObject* resolveObj;
	char* objectName;
	Object* object;
	__int16 objectID;
	__int16 objectStatus;
};

struct ObjectEntry
{
	char* name;
	int trackerIndex;
};

struct ObjectList
{
	int numObjects;
#ifdef TR8
	int size;
#endif
	ObjectEntry object[1];
};

ObjectTracker* STREAM_GetObjectTrackerByName(char* name);
bool STREAM_PollLoadQueue();