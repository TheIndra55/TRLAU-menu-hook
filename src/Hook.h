#pragma once

#include <memory>
#include <map>

#include "menu/Menu.h"
#include "modules/Module.h"

class Hook
{
private:
	std::unique_ptr<Menu> m_menu;
	std::map<std::size_t, std::shared_ptr<Module>> m_modules;

	void PostInitialize();

	void RegisterModules();

	void OnMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	void OnFrame();
	void OnLoop();

public:
	Hook();

	void Initialize();
	void OnDevice();

	// These need to be defined here, else the linker becomes angry

	// Adds a module
	template<typename T>
	void RegisterModule()
	{
		auto mod = std::make_shared<T>();
		m_modules.insert({ typeid(T).hash_code(), mod });
	}

	// Gets all modules
	const auto& GetModules() const noexcept
	{
		return m_modules;
	}

	// Get a module by T
	template<typename T>
	std::shared_ptr<T> GetModule() const
	{
		auto it = m_modules.find(typeid(T).hash_code());
		return it != m_modules.end() ? std::dynamic_pointer_cast<T>(it->second) : nullptr;
	}

	static Hook& GetInstance() noexcept;
};