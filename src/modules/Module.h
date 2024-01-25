#pragma once

#include <Windows.h>

class Module
{
public:
	// Called during drawing of the main menu bar, can be used to add menu items
	virtual void OnMenu() { };

	// Called during menu drawing, used for ImGui draw code
	virtual void OnDraw() { };

	// Called just before a frame ends, Font::Flush to be specific
	virtual void OnFrame() { };

	// Called every frame before the game loop
	virtual void OnLoop() { };

	// Called when a message is processed by the window procedure
	virtual void OnInput(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) { };
};