#pragma once

#include "Module.h"

class Skew : public Module
{
private:
	void ToggleSkew();
	void Process(UINT msg, WPARAM wParam);

public:
	virtual void OnInput(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};