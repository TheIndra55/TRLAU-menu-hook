#pragma once

class MouseHook
{
public:
	static void Init() noexcept;
	static void DisableCursorLock(bool disable) noexcept;
};