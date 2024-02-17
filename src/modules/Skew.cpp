#include "Skew.h"

#include "instance/Instances.h"
#include "game/Game.h"

Skew::Skew()
{
	UpdateLayout();
}

void Skew::ToggleSkew() const noexcept
{
	auto tracker = Game::GetGameTracker();

	// Ignore if there's no player instance
	if (tracker->playerInstance == nullptr)
	{
		return;
	}

	tracker->cheatMode = !tracker->cheatMode;

#ifndef TR8
	INSTANCE_Post(tracker->playerInstance, 1048592, tracker->cheatMode);
#else
	INSTANCE_Post(tracker->playerInstance, 12, tracker->cheatMode);
#endif
}

void Skew::Process(UINT msg, WPARAM wParam) const noexcept
{
	auto player = Game::GetPlayerInstance();
	auto tracker = Game::GetGameTracker();

	auto speed = m_speed.GetValue() * tracker->timeMult;

	// TODO less hacky way? use game input system perhaps
	if (msg == WM_KEYDOWN && wParam == (m_isAzerty ? 0x41 /* A */ : 0x51 /* Q */))
	{
		player->position.z += speed;
	}

	if (msg == WM_KEYDOWN && wParam == (m_isAzerty ? 0x57 /* W */ : 0x5A /* Z */))
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

	if (msg == WM_INPUTLANGCHANGE)
	{
		UpdateLayout();
	}

	auto tracker = Game::GetGameTracker();

	if (tracker->cheatMode)
	{
		Process(msg, wParam);
	}
}

void Skew::UpdateLayout() noexcept
{
	auto layout = GetKeyboardLayout(0);
	auto id = LOWORD(layout);

	m_isAzerty = id == 2060 || id == 1036;
}