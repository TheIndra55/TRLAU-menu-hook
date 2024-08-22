#pragma once

#include "cdc/math/Vector.h"
#include "cdc/math/Matrix.h"

#ifndef TR8
struct CameraCore
{
	cdc::Vector3 position;
	cdc::Vector3 effectPosition;

	cdc::Matrix vvNormalWorVecMatf[2];
	cdc::Matrix defaultvvNormalWorVecMatf[2];
	cdc::Matrix wcTransformf;
	cdc::Matrix wcTransformNoShakef;
	cdc::Matrix wcTransform2f;
	cdc::Matrix cwTransform2f;

	cdc::Vector3 viewVolumeNormal[5];

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