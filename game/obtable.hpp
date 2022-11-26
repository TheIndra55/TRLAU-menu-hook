#pragma once

#if TRAE
	#define gObjectList *(ObjectList**)0x842C70
#elif TR7
	#define gObjectList *(ObjectList**)ADDR(0x10F9110, 0x10EFC90)
#elif TR8
	#define gObjectList *(ObjectList**)0xDB94D0
#endif

struct ObjectEntry
{
#if TRAE || TR7
	char* name;
	int index;
#elif TR8
	int index;
	char* name;
#endif
};

struct ObjectList
{
	int numObjects;
	ObjectEntry objects[1];
};