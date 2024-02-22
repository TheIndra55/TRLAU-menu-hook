#pragma once

#include "Module.h"

class Debug : public Module
{
private:
	bool m_drawDebug = false;

	void Initialize() const noexcept;

public:
	Debug();

	bool IsDrawDebug() const noexcept { return m_drawDebug; }

	void OnPostInitialize();
	void OnMenu();
};