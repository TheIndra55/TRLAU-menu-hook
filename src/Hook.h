#pragma once

#include <memory>

#include "menu/Menu.h"

class Hook
{
private:
	std::unique_ptr<Menu> m_menu;

	void Initialize();
	void PostInitialize();

	void OnMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

public:
	Hook();

	void OnDevice();

	static Hook& GetInstance();
};