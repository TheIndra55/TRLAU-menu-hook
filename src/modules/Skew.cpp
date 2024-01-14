#include "Skew.h"

#include "instance/Instances.h"
#include "game/Game.h"

void Skew::ToggleSkew()
{
	auto tracker = Game::GetGameTracker();

	tracker->cheatMode = !tracker->cheatMode;
	Instances::Post(tracker->playerInstance, 1048592, tracker->cheatMode);
}

void Skew::Process(UINT msg, WPARAM wParam)
{
	// TODO different keyboard layouts
	// TODO configurable horizontal speed

	auto player = Game::GetPlayerInstance();
	auto tracker = Game::GetGameTracker();

	auto speed = 200.f * tracker->timeMult;

	if (msg == WM_KEYDOWN && wParam == 0x51)
	{
		player->position.z += speed;
	}

	if (msg == WM_KEYDOWN && wParam == 0x5A)
	{
		player->position.z -= speed;
	}
}

void Skew::OnInput(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_KEYUP && wParam == VK_F2)
	{
		ToggleSkew();
	}

	auto tracker = Game::GetGameTracker();

	if (tracker->cheatMode)
	{
		Process(msg, wParam);
	}
}