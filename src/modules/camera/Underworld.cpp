#if TR8

#define _USE_MATH_DEFINES
#include <cmath>

#include "FreeCamera.h"
#include "game/Camera.h"
#include "game/Game.h"
#include "util/Hooking.h"
#include "cdc/math/Math.h"

static cdc::Vector3 RotationToDirection(cdc::Euler* rotation, float distance)
{
	cdc::Matrix m;
	m.Build(rotation);

	cdc::Vector3 v = { 0.f, -distance, 0.f };

	// TODO fixme
	return cdc::Mul3x3(&m, &v);
}

void UnderworldCamera::ToggleMode()
{
	FreeCameraBase::ToggleMode();

	if (m_mode == Enabled)
	{
		CAMERA_SetMode(11);

		// Set the camera position to the player position
		auto camera = CAMERA_GetCamera();
		camera->transform.col3 = Game::GetPlayerInstance()->position;
	}

	if (m_mode == Disabled)
	{
		// Restore the gameplay camera
		Hooking::ThisCall(0x5EDF50, 0xE804F0 /* CameraManager */);
	}
}

void UnderworldCamera::OnControl()
{
	auto camera = CAMERA_GetCamera();
	m_position = camera->transform.col3;

	FreeCameraBase::OnControl();

	// Apply the rotation and position
	camera->transform.Build(&m_rotation);
	camera->transform.col3 = m_position;
}

void UnderworldCamera::Rotate(float x, float z)
{
	m_rotation.x -= x;
	m_rotation.z -= z;
}

void UnderworldCamera::Rotate(float y)
{
	m_rotation.y += y;
}

void UnderworldCamera::MoveForward(float distance)
{
	auto forward = RotationToDirection(&m_rotation, distance);

	m_position += &forward;
}

void UnderworldCamera::MoveLeft(float distance)
{
	auto rotation = m_rotation;
	rotation.x = 0.f;
	rotation.y = 0.f;
	rotation.z += static_cast<float>(M_PI) / 2.f;

	auto forward = RotationToDirection(&rotation, distance);

	m_position += &forward;
}

void UnderworldCamera::MoveUp(float distance)
{
	m_position.z += distance;
}

#endif