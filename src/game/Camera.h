#pragma once

#include "cdc/math/Vector.h"
#include "cdc/math/Matrix.h"

#ifndef TR8
struct CameraCore
{
	cdc::Vector3 position;
	char pad1[608];
	cdc::Euler rotation;
	char pad2[128];
};

struct Camera : CameraCore
{
	char pad3[12];

	int flags;
	int lock;
	__int16 mode;
};
#else
struct Camera
{
	char pad1[16];

	cdc::Matrix transform;
	cdc::Matrix invTransform;
	cdc::Vector3 position;
	cdc::Quat orientation;

	float fov;
};
#endif

Camera* CAMERA_GetCamera();
void CAMERA_CalcPosition(cdc::Vector3* position, cdc::Vector3* base, cdc::Euler* rotation, float distance);

#ifdef TR8
void CAMERA_SetMode(int mode);
#endif