#pragma once

#include <memory>
#include <list>

#include "menu/Menu.h"
#include "modules/Module.h"

class Hook
{
private:
	std::unique_ptr<Menu> m_menu;
	std::list<std::shared_ptr<Module>> m_modules;

	void Initialize();
	void PostInitialize();

	template<typename T>
	void RegisterModule();
	void RegisterModules();

	void OnMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	void OnFrame();

public:
	Hook();

	void OnDevice();

	const auto& GetModules() { return m_modules; }

	static Hook& GetInstance();
};