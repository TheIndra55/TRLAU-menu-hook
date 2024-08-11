#pragma once

#include "instance/Instance.h"

struct SweepAndPrune;
struct SAPBox;

class MarkupManager
{
public:
#ifndef TR8
	SweepAndPrune* m_sap;
#endif

	NodeType m_freeBoxList;
	NodeType m_dynamicBoxList;
	NodeType m_staticBoxList;

	static MarkupManager* GetInstance() noexcept;
};

struct PolyLine
{
	int numPoints;
	cdc::Vector3 point[1];
};

struct CameraAntic
{
	int UseAnticCamera;
	int newDTPCameraAnticDataID[5];
};

// Markup flags, compatible with both Legend and Anniversary
enum MUD_FLAGS : unsigned int
{
	MUD_FLAG_NONE,
	MUD_FLAG_DISABLED			= 0x80000000,

	MUD_FLAG_LEDGEGRAB			= 1 << 30,
	MUD_FLAG_JUMPLANDING		= 1 << 29,
	MUD_FLAG_ENEMYDROPDOWN		= 1 << 28,
	MUD_FLAG_WALLVERTPOLE		= 1 << 27,
	MUD_FLAG_VERTPOLE			= 1 << 26,
	MUD_FLAG_HORIZPOLE			= 1 << 25,
	MUD_FLAG_ZIPLINE			= 1 << 24,
	MUD_FLAG_MANTLEDOWN			= 1 << 23,
	MUD_FLAG_LADDER				= 1 << 22,
	MUD_FLAG_CLOSEJUMPTARGET	= 1 << 21,
	MUD_FLAG_WATER				= 1 << 20,
	MUD_FLAG_ROPE				= 1 << 19,
	MUD_FLAG_PERCH				= 1 << 18,
	MUD_FLAG_ATTACHPOINTS		= 1 << 17,
	MUD_FLAG_ENEMY_JUMP_UP		= 1 << 16,
	MUD_FLAG_ENEMY_JUMP_DOWN	= 1 << 15,
	MUD_FLAG_ENEMY_JUMP_ACROSS	= 1 << 14,
	MUD_FLAG_PATTERN_INTERACT	= 1 << 13,

	MUD_FLAG_WRAPS				= 1 << 2,
	MUD_FLAG_INSTANCE			= 1 << 1,
	MUD_FLAG_POLYLINE			= 1 << 0,

	MUD_FLAG_ALL_TYPES			= MUD_FLAG_ENEMY_JUMP_ACROSS | MUD_FLAG_ENEMY_JUMP_DOWN | MUD_FLAG_ENEMY_JUMP_UP | MUD_FLAG_PERCH | MUD_FLAG_ROPE | MUD_FLAG_WATER | MUD_FLAG_CLOSEJUMPTARGET | MUD_FLAG_LADDER | MUD_FLAG_MANTLEDOWN | MUD_FLAG_ZIPLINE | MUD_FLAG_HORIZPOLE | MUD_FLAG_VERTPOLE | MUD_FLAG_WALLVERTPOLE | MUD_FLAG_ENEMYDROPDOWN | MUD_FLAG_JUMPLANDING | MUD_FLAG_LEDGEGRAB | MUD_FLAG_DISABLED,
	MUD_FLAG_ALL				= 0xFFFFFFFF,
};

#ifndef TR8
struct MarkUp
{
	int OverrideMovementCamera;
	int DTPCameraDataID;
	int DTPMarkupDataID;

#ifdef TRAE
	int AnimatedSegment;
	CameraAntic cameraAntic;
#endif

	unsigned int flags;

	__int16 introID;
	__int16 markupID;

	float pos[3];
	__int16 bbox[6];

	PolyLine* polyLine;
};
#else
struct MarkUp
{
	int field_0;
	int field_4;
	int field_8;
	int field_C;
	int field_10;
	int field_14;
	int field_18;
	int field_1C;
	int field_20;
	int field_24;
	int field_28;
	int field_2C;
	int field_30;
	float pos[3];
	float bbox[6];
	PolyLine* polyLine;
};

#endif

#pragma pack(push, 16)
class MarkUpBox : NodeType
{
public:
	SAPBox* sapbox;

#ifdef TRAE
	MarkUp* markup;
	cdc::Vector3 velocity;
	Instance* instance;
	int instanceID;
#else
	Instance* instance;
	cdc::Vector velocity;
	MarkUp* markup;
#endif

#ifdef TR8
	int field_24;
#endif
	unsigned int flags;

	float attach_delay;
	float attach_frames;

#ifdef TRAE
	unsigned int matrix_index;
#endif

	unsigned int debugColor;

	int nSegments;
	void** associateRef;

	cdc::Vector3* GetSegmentPos(cdc::Vector3* result, int n) const noexcept;
};
#pragma pack(pop)