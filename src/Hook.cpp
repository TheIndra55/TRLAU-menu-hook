#include <MinHook.h>

#include "Hook.h"

#include "cdc/render/PCDeviceManager.h"

static bool(*s_D3D_Init)();

static bool D3D_Init()
{
	// Initialize the device
	auto ret = s_D3D_Init();

	Hook::GetInstance().OnDevice();

	return ret;
}

Hook::Hook() : m_menu(nullptr)
{
	Initialize();
}

void Hook::Initialize()
{
	MH_Initialize();
	MH_CreateHook((void*)0x4153E0, D3D_Init, (void**)&s_D3D_Init);
	MH_EnableHook(MH_ALL_HOOKS);
}

void Hook::OnDevice()
{
	// Assign the DeviceManager instance
	cdc::PCDeviceManager::s_pInstance = *(cdc::PCDeviceManager**)0xA6669C;

	// Creat the menu
	m_menu = std::make_unique<Menu>();
}

Hook& Hook::GetInstance()
{
	static Hook instance;
	return instance;
}