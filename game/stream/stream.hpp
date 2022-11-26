#pragma once

#include "../../Game.hpp"

#if TRAE
	#define gUnitList *(StreamUnitList**)0x8AF44C
#elif TR7
	#define gUnitList *(StreamUnitList**)ADDR(0x1162C54, 0x1159314)
#elif TR8
	#define gUnitList *(StreamUnitList**)0xDBA188
#endif

struct UnitInfo
{
	char name[MAX_UNIT_LEN];
	void* objectList;
};

struct StreamUnitList
{
	int numUnits;
	UnitInfo units[1];
};