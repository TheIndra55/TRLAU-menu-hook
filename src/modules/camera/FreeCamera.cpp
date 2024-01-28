#include "FreeCamera.h"

#include "input/Input.h"
#include "game/Game.h"
#include "render/Font.h"

FreeCameraBase::FreeCameraBase() : m_vKeys()
{
}

void FreeCameraBase::ToggleMode()
{
	// Switch between disabled -> enabled -> no control
	m_mode = m_mode == Disabled ? Enabled : m_mode == NoControl ? Disabled : NoControl;

	// Disable player control when the free camera is enabled
	Input::DisablePlayerControl(m_mode == Enabled);
}

void FreeCameraBase::OnInput(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_KEYUP && wParam == VK_F4)
	{
		ToggleMode();
	}

	// 1-3 virtual keys, kinda hack.. TODO global thing like this?
	if (msg == WM_KEYDOWN && wParam >= 0x31 && wParam <= 0x33)
	{
		m_vKeys[wParam - 0x31] = true;
	}

	if (msg == WM_KEYUP && wParam >= 0x31 && wParam <= 0x33)
	{
		m_vKeys[wParam - 0x31] = false;
	}
}

void FreeCameraBase::OnControl()
{
	auto input = Input::GetInputSystem();
	auto gameTracker = Game::GetGameTracker();

	// Camera rotation based on mouse/controller axis
	auto rotX = input->GetAxisValue(17) * gameTracker->timeMult;
	auto rotZ = input->GetAxisValue(16) * gameTracker->timeMult;

	Rotate(rotX, rotZ);

	// Camera roll
	if (m_vKeys[0] || m_vKeys[2])
	{
		// TODO
		auto roll = m_vKeys[0] ? 0.02454f : -0.02454f;

		Rotate(roll * gameTracker->timeMult);
	}

	// Change the speed depending on if shift/alt is pressed
	auto shift = Input::IsInputActionPressed(18);
	auto control = Input::IsInputActionPressed(23);

	auto speed = shift ? 200.f : control ? 20.f : 80.f;

	// Camera forward/backward
	if (Input::IsInputActionPressed(1) || Input::IsInputActionPressed(2))
	{
		auto distance = Input::IsInputActionPressed(1) ? -speed : speed;

		MoveForward(distance * gameTracker->timeMult);
	}

	// Camera left/right
	if (Input::IsInputActionPressed(3) || Input::IsInputActionPressed(4))
	{
		auto distance = Input::IsInputActionPressed(3) ? -speed : speed;

		MoveLeft(distance * gameTracker->timeMult);
	}

	// Camera up/down
	if (Input::IsInputActionPressed(16) || Input::IsInputActionPressed(17))
	{
		auto distance = Input::IsInputActionPressed(16) ? -speed : speed;

		MoveUp(distance * gameTracker->timeMult);
	}
}

void FreeCameraBase::OnLoop()
{
	if (m_mode == Enabled)
	{
		OnControl();
	}
}