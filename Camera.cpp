#define _USE_MATH_DEFINES
#include <cmath>

#include "Hooking.hpp"
#include "Camera.hpp"

void(__cdecl* origCAMERA_Process)(Camera* camera);

#if TR8
void(__cdecl* origGAMELOOP_Process)(int a1);
void(__thiscall* _setCameraRotation)(int _this, cdc::Vector* a2);
#endif

bool(__cdecl* EVENT_InputActionOn)(int action);
void(__cdecl* CAMERA_CalcPosition)(cdc::Vector* position, cdc::Vector* base, cdc::Vector* rotation, float distance);
double(__thiscall* InputSystem_GetAxisValue)(int _this, int a2);

void CAMERA_Fly(Camera* camera)
{
	auto speed = EVENT_InputActionOn(18);

	if (EVENT_InputActionOn(1)) // forward
	{
		CAMERA_CalcPosition(&camera->position, &camera->position, &camera->rotation, speed ? -50.f : -20.f);
	}

	if (EVENT_InputActionOn(2)) // backward
	{
		CAMERA_CalcPosition(&camera->position, &camera->position, &camera->rotation, speed ? 50.f : 20.f);
	}

	auto a = InputSystem_GetAxisValue(*(int*)0x008551A0, 17);
	auto b = InputSystem_GetAxisValue(*(int*)0x008551A0, 16);

	camera->rotation.x -= static_cast<float>(a);
	camera->rotation.z -= static_cast<float>(b);
}

void __cdecl CAMERA_Process(Camera* camera)
{
	origCAMERA_Process(camera);

	if (*(__int16*)0x850984 == 7)
	{
		CAMERA_Fly(camera);
	}
}

#if TR8
cdc::Vector RotationToDirection(cdc::Vector rotation)
{
	auto x = rotation.x;

	// thanks to sunbeam906
	// thanks to Xwilarg
	auto xx = -sin(rotation.z);
	auto yy = cos(rotation.z);
	auto zz = (-sin(x) * cos(x)); // FIXME better calculation so you can go straight down/up

	return cdc::Vector{ static_cast<float>(xx), static_cast<float>(yy), static_cast<float>(zz) };
}

cdc::Vector CameraForward(cdc::Vector position, cdc::Vector rotation, float distance)
{
	auto direction = RotationToDirection(rotation);

	auto x = position.x + direction.x * distance;
	auto y = position.y + direction.y * distance;
	auto z = position.z + direction.z * distance;

	return cdc::Vector{ x, y, z };
}

// our own TR8 implementation of EVENT_InputActionOn
bool IsActionPressed(int action)
{
	auto input = *(int*)0x00A02B68; /* INPUT_instances */
	auto actions = **(int**)(input + 0x18);

	// im not responsible if this overflows
	return (*(int*)(actions + (24 * action)) & 4) == 4;
}

void UpdateCamera(int camera, cdc::Vector position, cdc::Vector rotation)
{
	auto ptr = camera + 0x10;

	_setCameraRotation(ptr, &rotation);
	*reinterpret_cast<cdc::Vector*>(camera + 0x40) = position;
}

void __cdecl GAMELOOP_Process(int a1)
{
	origGAMELOOP_Process(a1);

	if (Hooking::GetInstance().GetMenu()->IsFreecam())
	{
		auto speed = IsActionPressed(34);
		auto camera = *reinterpret_cast<int*>(0xE80534);

		static cdc::Vector rotation{ 0.0f, 0.0f, 0.0f, 0.f };

		auto position = *reinterpret_cast<cdc::Vector*>(camera + 0x40);

		if (IsActionPressed(2))
		{
			position = CameraForward(position, rotation, speed ? 50.f : 20.f);
		}

		if (IsActionPressed(4))
		{
			position = CameraForward(position, rotation, speed ? -50.f : -20.f);
		}

		auto a = InputSystem_GetAxisValue(*(int*)0x00A02B68, 17);
		auto b = InputSystem_GetAxisValue(*(int*)0x00A02B68, 16);

		rotation.x -= static_cast<float>(a);
		rotation.z -= static_cast<float>(b);
		UpdateCamera(camera, position, rotation);
	}
}
#endif

void InstallCameraHooks()
{
#if TRAE
	MH_CreateHook((void*)0x481D70, CAMERA_Process, (void**)&origCAMERA_Process);

	EVENT_InputActionOn = reinterpret_cast<bool(__cdecl*)(int)>(0x42F740);
	CAMERA_CalcPosition = reinterpret_cast<void(__cdecl*)(cdc::Vector*, cdc::Vector*, cdc::Vector*, float)>(0x00491320);
	InputSystem_GetAxisValue = reinterpret_cast<double(__thiscall*)(int, int)>(0x004E38C0);
#elif TR8
	// no known CAMERA_Process in Underworld so hook GAMELOOP_Process
	MH_CreateHook((void*)0x005DFBE0, GAMELOOP_Process, (void**)&origGAMELOOP_Process);

	InputSystem_GetAxisValue = reinterpret_cast<double(__thiscall*)(int, int)>(0x00480E50);
	_setCameraRotation = reinterpret_cast<void(__thiscall*)(int, cdc::Vector*)>(0x0049DD70);
#endif

	MH_EnableHook(MH_ALL_HOOKS);
}