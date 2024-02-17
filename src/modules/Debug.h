#pragma once

#include "Module.h"

class Debug : public Module
{
private:
	bool m_drawDebug = false;

	void Initialize();

public:
	Debug();

	bool IsDrawDebug() { return m_drawDebug; }

	void OnPostInitialize();
	void OnMenu();
};