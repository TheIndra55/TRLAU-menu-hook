#pragma once

#include "Object.h"
#include "Animation.h"

#include "cdc/math/Vector.h"
#include "cdc/math/Matrix.h"

struct NodeType
{
	NodeType* prev;
	NodeType* next;
};

struct Intro
{
};

struct IntroData
{
};

struct ObjectData
{
	unsigned __int16 version;
	unsigned __int16 family;
	unsigned __int16 id;
	unsigned __int16 type;
};

struct HModel;
struct SoundInstanceData;
struct Body;
struct AnimatedGeoms;
struct CharacterProxy;

struct Instance;

#ifndef TR8
struct BaseInstance
{
	NodeType node;

	Instance* next;
	Instance* prev;

	cdc::Vector3 position;
	cdc::Vector3 oldPos;

	cdc::Euler rotation;
	cdc::Euler oldRotation;

	cdc::Vector3 scale;
	cdc::Vector3 shadowPosition;
	cdc::Vector3 centerOfMass;

	cdc::Matrix* matrix;
	cdc::Matrix* oldMatrix;

	char pad1[12];

	Object* object;
	Intro* intro;

	char pad2[64];

	unsigned int noDrawGroups;

	char pad3[20];

	AnimComponent* animComponent;

	char pad4[8];
};

struct Instance : BaseInstance
{
	char pad1[192];

	void* data;

	char pad2[12];

	int introUniqueID;

	char pad3[104];

	void* extraData;
};
#else
struct Instance
{
	NodeType node;

	Instance* next;
	Instance* prev;

	Object* object;

	char pad1[12];

	cdc::Vector3 position;
	cdc::Euler rotation;

	char pad2[24];

	int introUniqueID;
};
#endif

void INSTANCE_Post(Instance* instance, int message, int data);
void INSTANCE_HideUnhideDrawGroup(Instance* instance, int drawGroup, int on);
void INSTANCE_ReallyRemoveInstance(Instance* instance, int reset, bool keepSave);

Instance* INSTANCE_BirthObjectNoParent(unsigned int unitID, cdc::Vector3* position, cdc::Euler* rotation, IntroData* introData, Object* object, int modelNum, int initEffects);