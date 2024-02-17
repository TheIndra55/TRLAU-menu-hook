#include "Input.h"

#include "util/Hooking.h"

void Input::DisableInput(bool disable) noexcept
{
	*(bool*)GET_ADDRESS(0x1101689, 0x8551A9, 0xA02B79) = disable;
}

void Input::DisablePlayerControl(bool disable) noexcept
{
	*(bool*)GET_ADDRESS(0xF15AB4, 0x666C34, 0x8AB4E6) = !disable;
}

bool Input::IsPlayerControlEnabled() noexcept
{
	return *(bool*)GET_ADDRESS(0xF15AB4, 0x666C34, 0x8AB4E6);
}

bool Input::IsInputActionPressed(int action) noexcept
{
	auto inputSystem = GetInputSystem();

	return (inputSystem->m_pActionMapper->m_pActionResults[action].state & 4) == 4;
}

InputSystem* Input::GetInputSystem() noexcept
{
	return *(InputSystem**)GET_ADDRESS(0x1101680, 0x8551A0, 0xA02B68);
}

float InputSystem::GetAxisValue(int axisID) const noexcept
{
	auto addr = GET_ADDRESS(0x4E3FC0, 0x4E38C0, 0x480E50);

	return Hooking::ThisCallReturn<float>(addr, this, axisID);
}