#pragma once

#include "Object.h"

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
struct AnimComponent;
struct CharacterProxy;

struct Instance;

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

	char pad3[32];
};

struct Instance : BaseInstance
{
	char pad1[192];

	void* data;

	char pad2[12];

	int introUniqueID;
};

void INSTANCE_Post(Instance* instance, int message, int data);
void INSTANCE_HideUnhideDrawGroup(Instance* instance, int drawGroup, int on);