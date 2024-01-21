#pragma once

struct Model
{
};

struct AnimListEntry;
struct AnimFxHeader;
struct AnimScriptObject;
struct ObjectBase;

#ifndef TR8
struct Object
{
	int oflags;
	int oflags2;

	int uniqueID;
	unsigned int guiID;
	int functionTableID;

	void* obsoleteSoundBank;

	__int16 numModels;
	__int16 numAnims;
	__int16 numAnimPatterns;

	Model** modelList;

	AnimListEntry* animList;
	AnimFxHeader** animFXList;
	AnimScriptObject** animPatternList;

	int introDist;
	int vvIntroDist;
	int removeDist;
	int vvRemoveDist;

	ObjectBase* baseData;

	void* data;
	char* name;
};
#else
struct Object
{
	char pad1[58];

	__int16 numModels;
	__int16 numAnims;

	char pad2[34];

	char* name;
};
#endif