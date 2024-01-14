#pragma once

#include <Windows.h>

class Module
{
public:
	virtual void OnMenu() { };
	virtual void OnDraw() { };
	virtual void OnInput(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) { };
};