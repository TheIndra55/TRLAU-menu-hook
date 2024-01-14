#pragma once

#include <memory>

#include "menu/Menu.h"

class Hook
{
private:
	std::unique_ptr<Menu> m_menu;

	void Initialize();

public:
	Hook();

	void OnDevice();

	static Hook& GetInstance();
};