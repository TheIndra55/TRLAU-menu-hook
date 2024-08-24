#include <MinHook.h>
#include <Hooking.Patterns.h>

#include "Hook.h"
#include "Options.h"
#include "input/MessageHook.h"
#include "instance/Instances.h"
#include "game/Game.h"
#include "render/Font.h"
#include "game/GameLoop.h"
#include "render/DrawBatcher.h"

// Modules
#include "modules/MainMenu.h"
#include "modules/Instance.h"
#include "modules/Skew.h"
#include "modules/Render.h"
#include "modules/Log.h"
#include "modules/ScriptLog.h"
#include "modules/Level.h"
#include "modules/ModLoader.h"
#include "modules/Patches.h"
#include "modules/Frontend.h"
#include "modules/Draw.h"
#include "modules/Debug.h"
#include "modules/Materials.h"
#include "modules/camera/FreeCamera.h"

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
}

void Hook::Initialize()
{
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

	Font::OnFlush(std::bind(&Hook::OnFrame, this));
	GameLoop::OnLoop(std::bind(&Hook::OnLoop, this));

	// Post initialize all modules
	for (auto& [hash, mod] : m_modules)
	{
		mod->OnPostInitialize();
	}
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

#ifdef BATCH_DRAW_CALLS
	DrawBatcher::GetInstance()->Flush();
#endif
}

void Hook::OnLoop()
{
	for (auto& [hash, mod] : m_modules)
	{
		mod->OnLoop();
	}
}

void Hook::OnDevice()
{
	// Assign the DeviceManager instance
	cdc::PCDeviceManager::s_pInstance = *(cdc::PCDeviceManager**)s_deviceManager;

	// Initialize the hook
	PostInitialize();
}

void Hook::RegisterModules()
{
	// Register these first
	RegisterModule<Log>();
	RegisterModule<Options>();

	RegisterModule<MainMenu>();
	RegisterModule<InstanceModule>();
	RegisterModule<Skew>();
	RegisterModule<ModLoader>();
	RegisterModule<Patches>();
	RegisterModule<FreeCamera>();
	RegisterModule<Draw>();

#ifndef TR8
	RegisterModule<LevelModule>();
	RegisterModule<Frontend>();
	RegisterModule<Render>();
	RegisterModule<Debug>();
#else
	RegisterModule<ScriptLog>();
	RegisterModule<Materials>();
#endif
}

Hook& Hook::GetInstance() noexcept
{
	static Hook instance;
	return instance;
}