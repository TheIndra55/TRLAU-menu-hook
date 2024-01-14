#pragma once

#include <Windows.h>

#include "cdc/render/PCInternalResource.h"

class Menu : public cdc::PCInternalResource
{
private:
	bool m_initialized = false;
	bool m_focus = false;

public:
	Menu();

	void OnPresent();
	void OnMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	bool OnCreateDevice();
	void OnDestroyDevice();

	void SetFocus(bool focus);
	bool HasFocus();

private:
	void Draw();
};