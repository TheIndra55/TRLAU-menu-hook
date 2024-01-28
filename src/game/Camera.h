#pragma once

#include "cdc/math/Vector.h"

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

Camera* CAMERA_GetCamera();
void CAMERA_CalcPosition(cdc::Vector3* position, cdc::Vector3* base, cdc::Euler* rotation, float distance);