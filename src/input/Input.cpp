#include "Input.h"

#include "util/Hooking.h"

void Input::DisableInput(bool disable)
{
	*(bool*)GET_ADDRESS(0x1101689, 0x8551A9, 0xA02B79) = disable;
}