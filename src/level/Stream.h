#pragma once

#include "instance/Object.h"

#if TRAE || TR7
#define MAX_UNIT_NAME_LENGTH 20
#else
#define MAX_UNIT_NAME_LENGTH 128
#endif

struct ResolveObject;

struct ObjectTracker
{
	ResolveObject* resolveObj;
	char* objectName;
	Object* object;
	__int16 objectID;
	__int16 objectStatus;
};

ObjectTracker* STREAM_GetObjectTrackerByName(char* name);
bool STREAM_PollLoadQueue();