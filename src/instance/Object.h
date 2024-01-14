#pragma once

struct Model
{
};

struct AnimListEntry;
struct AnimFxHeader;
struct AnimScriptObject;
struct ObjectBase;

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