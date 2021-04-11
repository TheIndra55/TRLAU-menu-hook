#include "Hooking.hpp"
#include "Camera.hpp"

void(__cdecl* origCAMERA_Process)(Camera* camera);

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

void InstallCameraHooks()
{
#if TRAE
	MH_CreateHook((void*)0x481D70, CAMERA_Process, (void**)&origCAMERA_Process);

	EVENT_InputActionOn = reinterpret_cast<bool(__cdecl*)(int)>(0x42F740);
	CAMERA_CalcPosition = reinterpret_cast<void(__cdecl*)(cdc::Vector*, cdc::Vector*, cdc::Vector*, float)>(0x00491320);
	InputSystem_GetAxisValue = reinterpret_cast<double(__thiscall*)(int, int)>(0x004E38C0);

	MH_EnableHook(MH_ALL_HOOKS);
#endif
}