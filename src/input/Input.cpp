#include "Input.h"

void Input::DisableInput(bool disable)
{
	*(bool*)0x8551A9 = disable;
}