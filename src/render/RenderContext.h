#pragma once

#include <functional>

class RenderContext
{
public:
	// Registers a callback called just before device present
	static void OnPresent(std::function<void()> callback) noexcept;
};