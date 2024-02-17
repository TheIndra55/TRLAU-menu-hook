#pragma once
 
#include "Module.h"
#include "Options.h"

class Patches : public Module
{
private:
	Option<bool> m_disableIntro{ "DisableIntro", true };
	Option<bool> m_noCinematicBars{ "NoCinematicBars", true };
	Option<bool> m_noMotionBlur{ "NoMotionBlur", false };

	void RemoveIntro() const noexcept;

public:
	Patches();

	bool IsNoMotionBlur() const noexcept { return m_noMotionBlur.GetValue(); }
	bool IsNoCinematicBars() const noexcept { return m_noCinematicBars.GetValue(); }

	void OnInput(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};