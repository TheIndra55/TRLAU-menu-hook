#define _USE_MATH_DEFINES
#include <cmath>

#include "FreeCamera.h"
#include "game/Camera.h"

void LegendCamera::ToggleMode()
{
	FreeCameraBase::ToggleMode();

	auto camera = CAMERA_GetCamera();

	if (m_mode == Enabled)
	{
		camera->mode = 7;
	}

	if (m_mode == Disabled)
	{
		// Switch back to gameplay camera
		camera->mode = 2;
	}
}

void LegendCamera::Rotate(float x, float z)
{
	auto camera = CAMERA_GetCamera();

	camera->rotation.x -= x;
	camera->rotation.z -= z;
}

void LegendCamera::Rotate(float y)
{
	auto camera = CAMERA_GetCamera();

	camera->rotation.y += y;
}

void LegendCamera::MoveForward(float distance)
{
	auto camera = CAMERA_GetCamera();

	CAMERA_CalcPosition(&camera->position, &camera->position, &camera->rotation, distance);
}

void LegendCamera::MoveLeft(float distance)
{
	auto camera = CAMERA_GetCamera();
	auto rotation = camera->rotation;

	rotation.x = 0.f;
	rotation.y = 0.f;
	rotation.z += static_cast<float>(M_PI) / 2.f;

	CAMERA_CalcPosition(&camera->position, &camera->position, &rotation, distance);
}

void LegendCamera::MoveUp(float distance)
{
	auto camera = CAMERA_GetCamera();

	camera->position.z += distance;
}