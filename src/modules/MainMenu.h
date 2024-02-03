#pragma once

#include "Module.h"
#include "Options.h"

class MainMenu : public Module
{
private:
	bool m_switchPlayerNextFrame = false;

	Option<bool> m_noWatermark{ "NoWatermark", false };

	void BirthObject(char* name);
	void SwitchPlayerCharacter(char* name = nullptr);

public:
	void OnDraw();
	void OnFrame();
	void OnLoop();
	void OnInput(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};