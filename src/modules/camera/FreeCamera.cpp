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
	auto rotX = input->GetAxisValue(INPUTAXIS_MOUSEY) * gameTracker->timeMult;
	auto rotZ = input->GetAxisValue(INPUTAXIS_MOUSEX) * gameTracker->timeMult;

	Rotate(rotX, rotZ);

	// Camera roll
	if (m_vKeys[0] || m_vKeys[2])
	{
		// TODO
		auto roll = m_vKeys[0] ? 0.02454f : -0.02454f;

		Rotate(roll * gameTracker->timeMult);
	}

	// Change the speed depending on if shift/alt is pressed
	auto shift = Input::IsInputActionPressed(INPUTACTION_CAMERA_FAST);
	auto control = Input::IsInputActionPressed(INPUTACTION_CAMERA_SLOW);

	auto speed = shift ? 200.f : control ? 20.f : 80.f;

	// Camera forward/backward
	if (Input::IsInputActionPressed(INPUTACTION_UP) || Input::IsInputActionPressed(INPUTACTION_DOWN))
	{
		auto distance = Input::IsInputActionPressed(INPUTACTION_UP) ? -speed : speed;

		MoveForward(distance * gameTracker->timeMult);
	}

	// Camera left/right
	if (Input::IsInputActionPressed(INPUTACTION_LEFT) || Input::IsInputActionPressed(INPUTACTION_RIGHT))
	{
		auto distance = Input::IsInputActionPressed(INPUTACTION_LEFT) ? -speed : speed;

		MoveLeft(distance * gameTracker->timeMult);
	}

	// Camera up/down
	if (Input::IsInputActionPressed(INPUTACTION_CAMERA_UP) || Input::IsInputActionPressed(INPUTACTION_CAMERA_DOWN))
	{
		auto distance = Input::IsInputActionPressed(INPUTACTION_CAMERA_UP) ? -speed : speed;

		MoveUp(distance * gameTracker->timeMult);
	}
}

#include "Hook.h"
#include "modules/Log.h"

void FreeCameraBase::OnLoop()
{
	if (m_mode == Enabled)
	{
		OnControl();
	}

	for (int i = 0; i < 100; i++)
	{
		if (Input::IsInputActionPressed(i))
		{
			Hook::GetInstance().GetModule<Log>()->WriteLine("%d", i);
		}
	}
}