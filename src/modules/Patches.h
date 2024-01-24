#pragma once
 
#include "Module.h"

class Patches : public Module
{
private:
	void RemoveIntro();

public:
	Patches();

	void OnInput(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};