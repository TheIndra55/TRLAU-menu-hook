#pragma once

#include "../Game.hpp"

struct PolyLine
{
	int numPoints;
	char pad[12];
	cdc::Vector points[1];
};

struct CameraAntic
{
	char pad[24];
};

struct MarkUp
{
	int OverrideMovementCamera;
	int DTPCameraDataID;
	int DTPMarkupDataID;
#if TRAE
	int AnimatedSegment;
	CameraAntic cameraAntic;
#endif
	unsigned int flags;
	__int16 introID;
	__int16 markupID;
	float position[3];
	__int16 bbox[6];
	PolyLine* polyLine;
};

struct MarkUpBox
{
	MarkUpBox* prev;
	MarkUpBox* next;
	void* sapbox;
#if TRAE
	MarkUp* markup;
	cdc::Vector velocity;
	Instance* instance;
	int instanceID;
#else
	Instance* instance;
	cdc::Vector velocity;
	MarkUp* markup;
#endif
	unsigned int flags;
	float attach_delay;
	float attach_frames;
#if TRAE
	unsigned int matrix_index;
#endif
	unsigned int debugColor;
	int nSegments;
	void* associateRef;
};
