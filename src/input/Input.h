#pragma once

class InputActionMapper
{
public:
	struct ActionResult
	{
		unsigned int state;

		char pad1[20];
	};

	ActionResult* m_pActionResults;
};

class InputSystem
{
public:
	unsigned int m_numButtons;
	unsigned int m_numAxis;

	char pad1[16];

	InputActionMapper* m_pActionMapper;

public:
	float GetAxisValue(int axisID);
};

class Input
{
public:
	static void DisableInput(bool disable);
	static void DisablePlayerControl(bool disable);
	static bool IsInputActionPressed(int action);

	static InputSystem* GetInputSystem();
};