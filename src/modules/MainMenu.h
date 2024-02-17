#pragma once

#include "Module.h"
#include "Options.h"

class MainMenu : public Module
{
private:
	bool m_switchPlayerNextFrame = false;
	bool m_noDeathFade = false;

	Option<bool> m_noWatermark{ "NoWatermark", false };

	void BirthObject(char* name) const noexcept;
	void SwitchUnit(char* name) const noexcept;
	void SwitchPlayerCharacter(char* name = nullptr) noexcept;

	static bool CheckDataFile(char* name) noexcept;

public:
	bool IsNoDeathFade() const noexcept { return m_noDeathFade; }

	void OnDraw();
	void OnFrame();
	void OnLoop();
	void OnInput(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};