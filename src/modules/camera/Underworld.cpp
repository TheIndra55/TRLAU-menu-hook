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
	//cdc::Matrix m;
	//m.Build(rotation);

	//cdc::Vector3 v = { 0.f, -distance, 0.f };

	//return cdc::Mul3x3(&m, &v);
	auto x = -sin(rotation->z);
	auto y = cos(rotation->z);
	auto z = cos(rotation->x);

	return cdc::Vector3{ static_cast<float>(x * -distance), static_cast<float>(y * -distance), static_cast<float>(z * -distance) };
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
		camera->fov = static_cast<float>(M_PI) / 2;

		m_rotation = { static_cast<float>(M_PI) / -2.f, 0.f, 0.f };
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

	cdc::OrthonormalInverse3x4(&camera->invTransform, &camera->transform);
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