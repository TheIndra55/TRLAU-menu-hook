#pragma once

#include <functional>

class GameLoop
{
public:
	static void OnLoop(std::function<void()> callback);
};