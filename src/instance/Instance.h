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

struct IntroData
{
	int type;
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
struct Intro
{
};

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
struct Intro
{
	char pad1[120];

	IntroData* data;
};

struct Instance
{
	NodeType node;

	Instance* next;
	Instance* prev;

	Object* object;
	int iflags;

	int field_18;
	int field_1C;

	cdc::Vector3 position;
	cdc::Euler rotation;

	int field_40;
	int field_44;
	int field_48;
	int field_4C;

	int data;
	int field_54;
	int introUniqueID;

	int field_5C;
	int field_60;
	int field_64;
	int field_68;
	int field_6C;
	int field_70;
	int field_74;
	int field_78;
	int field_7C;
	int field_80;
	int field_84;
	int field_88;
	int field_8C;
	int field_90;
	int field_94;
	int field_98;
	int field_9C;
	int field_A0;
	int field_A4;
	int field_A8;
	int field_AC;
	int field_B0;
	int field_B4;
	int field_B8;
	int field_BC;
	int field_C0;
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

	Intro* intro;

	int field_104;
	int field_108;
	int field_10C;
	int field_110;
	int field_114;
	int field_118;
	int field_11C;
	int field_120;
	int field_124;
	int field_128;
	int field_12C;
	int field_130;
	int field_134;
	int field_138;
	int field_13C;
	int field_140;
	int field_144;
	int field_148;
	int field_14C;
	int field_150;
	int field_154;
	int field_158;
	int field_15C;
	int field_160;
	int field_164;
	int field_168;
	int field_16C;
	int field_170;
	int field_174;
	int field_178;
	int field_17C;
	int field_180;
	int field_184;
	int field_188;
	int field_18C;
	int field_190;
	int field_194;
	int field_198;
	int field_19C;
	int field_1A0;
	int field_1A4;
	int field_1A8;
	int field_1AC;
	int field_1B0;
	int field_1B4;
	int field_1B8;
	int field_1BC;
	int field_1C0;
	int field_1C4;
	int field_1C8;
	int field_1CC;
	int field_1D0;
	int field_1D4;
	int field_1D8;
	int field_1DC;
	int field_1E0;
	int field_1E4;
	int field_1E8;
	int field_1EC;
	int field_1F0;
	int field_1F4;
	int field_1F8;
	int field_1FC;
	int field_200;
	int field_204;
	int field_208;
	int field_20C;
	int field_210;
	int field_214;
	int field_218;
	int field_21C;
	int field_220;
	int field_224;
	int field_228;
	int field_22C;
	int field_230;
	int field_234;
	int field_238;

	int noDrawGroups;

	int field_240;
	int field_244;
	int field_248;
	int field_24C;
	int field_250;
	int field_254;
	int field_258;
	int field_25C;
	int field_260;
	int field_264;
	int field_268;
	int field_26C;
	int field_270;
	int field_274;
	int field_278;
	int field_27C;

	AnimComponent* animComponent;

	int field_284;
	int field_288;
	int field_28C;
	int field_290;
	int field_294;
	int field_298;
	int field_29C;
	int field_2A0;
	int field_2A4;
	int field_2A8;
	int field_2AC;
	int field_2B0;
	int field_2B4;
	int field_2B8;
	int field_2BC;
	int field_2C0;
	int field_2C4;
	int field_2C8;
	int field_2CC;

	int extraData;

	int field_2D4;
	int field_2D8;
	int field_2DC;
	int field_2E0;
	int field_2E4;
};
#endif

void INSTANCE_Post(Instance* instance, int message, int data);
void INSTANCE_HideUnhideDrawGroup(Instance* instance, int drawGroup, int on);
void INSTANCE_ReallyRemoveInstance(Instance* instance, int reset, bool keepSave);

Instance* INSTANCE_BirthObjectNoParent(unsigned int unitID, cdc::Vector3* position, cdc::Euler* rotation, IntroData* introData, Object* object, int modelNum, int initEffects);