#include <MinHook.h>
#include <Hooking.Patterns.h>

#include "Hook.h"
#include "input/MessageHook.h"
#include "instance/Instances.h"
#include "game/Game.h"
#include "render/Font.h"

// Modules
#include "modules/MainMenu.h"
#include "modules/InstanceViewer.h"
#include "modules/Skew.h"
#include "modules/Render.h"
#include "modules/Draw.h"
#include "modules/Log.h"
#include "modules/ScriptLog.h"

#include "cdc/render/PCDeviceManager.h"

using namespace std::placeholders;

static bool(*s_D3D_Init)();

// Pointer to the cdc::DeviceManager::s_pInstance pointer
static void* s_deviceManager;

static bool D3D_Init()
{
	// Initialize the device
	auto ret = s_D3D_Init();

	Hook::GetInstance().OnDevice();

	return ret;
}

Hook::Hook() : m_menu(nullptr), m_modules()
{
	Initialize();
}

void Hook::Initialize()
{
	// Initialize MinHook
	MH_Initialize();

	// Register all modules
	RegisterModules();

#ifndef TR8
	auto match = hook::pattern("A1 ? ? ? ? 8B 0D ? ? ? ? 68 ? ? ? ? 50 E8").count(1);
	s_deviceManager = *match.get_first<void*>(7);
#else
	auto match = hook::pattern("8B 0D ? ? ? ? 8B 01 8B 15 ? ? ? ? 8B 00 68").count(1);
	s_deviceManager = *match.get_first<void*>(2);
#endif

	// Create the initial hook
	MH_CreateHook(match.get_first(), D3D_Init, (void**)&s_D3D_Init);
	MH_EnableHook(MH_ALL_HOOKS);
}

void Hook::PostInitialize()
{
	// Create the menu
	m_menu = std::make_unique<Menu>();

	// Register the message hook
	MessageHook::OnMessage(std::bind(&Hook::OnMessage, this, _1, _2, _3, _4));

#ifndef TR8
	Font::OnFlush(std::bind(&Hook::OnFrame, this));
#endif
}

void Hook::OnMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	m_menu->OnMessage(hWnd, msg, wParam, lParam);

	for (auto& [hash, mod] : m_modules)
	{
		mod->OnInput(hWnd, msg, wParam, lParam);
	}
}

void Hook::OnFrame()
{
	for (auto& [hash, mod] : m_modules)
	{
		mod->OnFrame();
	}
}

void Hook::OnDevice()
{
	// Assign the DeviceManager instance
	cdc::PCDeviceManager::s_pInstance = *(cdc::PCDeviceManager**)s_deviceManager;

	// Initialize the hook
	PostInitialize();
}

template<typename T>
void Hook::RegisterModule()
{
	auto mod = std::make_shared<T>();

	m_modules.insert({ typeid(T).hash_code(), mod });
}

void Hook::RegisterModules()
{
	RegisterModule<MainMenu>();
	RegisterModule<InstanceViewer>();
	RegisterModule<Skew>();

#ifndef TR8
	RegisterModule<Render>();
	RegisterModule<Draw>();
#else
	RegisterModule<ScriptLog>();
#endif

	RegisterModule<Log>();

}

Hook& Hook::GetInstance()
{
	static Hook instance;
	return instance;
}