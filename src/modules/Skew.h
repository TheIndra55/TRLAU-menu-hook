#pragma once

#include "Module.h"
#include "Options.h"

class Skew : public Module
{
private:
	bool m_isAzerty = false;

	void ToggleSkew() const noexcept;
	void Process(UINT msg, WPARAM wParam) const noexcept;
	void UpdateLayout() noexcept;

	Option<float> m_speed{ "SkewSpeed", 300.f };

public:
	Skew();

	virtual void OnInput(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};