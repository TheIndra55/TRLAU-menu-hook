#pragma once
 
#include "Module.h"
#include "Options.h"

class Patches : public Module
{
private:
	Option<bool> m_disableIntro{ "DisableIntro", true };
	Option<bool> m_noCinematicBars{ "NoCinematicBars", true };
	Option<bool> m_noMotionBlur{ "NoMotionBlur", false };

	void RemoveIntro();

public:
	Patches();

	void OnInput(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};