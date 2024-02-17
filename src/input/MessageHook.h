#pragma once

#include <Windows.h>
#include <functional>

class MessageHook
{
public:
	static void OnMessage(std::function<void(HWND, UINT, WPARAM, LPARAM)> callback) noexcept;
};