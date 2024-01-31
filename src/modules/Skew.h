#pragma once

#include "Module.h"
#include "Options.h"

class Skew : public Module
{
private:
	void ToggleSkew();
	void Process(UINT msg, WPARAM wParam);

	Option<float> m_speed{ "SkewSpeed", 300.f };

public:
	virtual void OnInput(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};