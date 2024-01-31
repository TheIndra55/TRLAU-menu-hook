#pragma once
 
#include "Module.h"
#include "Options.h"

class Patches : public Module
{
private:
	Option<bool> m_disableIntro{ "DisableIntro", true };

	void RemoveIntro();

public:
	Patches();

	void OnInput(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};