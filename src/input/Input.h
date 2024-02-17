#pragma once

enum INPUTACTION
{
	INPUTACTION_NONE,

	INPUTACTION_UP,
	INPUTACTION_DOWN,
	INPUTACTION_LEFT,
	INPUTACTION_RIGHT,
	INPUTACTION_DPAD_UP,
	INPUTACTION_DPAD_DOWN,
	INPUTACTION_DPAD_LEFT,
	INPUTACTION_DPAD_RIGHT,
	INPUTACTION_START,
	INPUTACTION_SELECT,

#ifndef TR8
	INPUTACTION_TRIANGLE = 16,
	INPUTACTION_SQUARE = 17,
	INPUTACTION_CIRCLE = 18,
	INPUTACTION_CROSS = 19,
	INPUTACTION_L1 = 20,
	INPUTACTION_R1 = 21,
	INPUTACTION_L2 = 22,
	INPUTACTION_R2 = 23,

	// Kinda hack, instead we should probably see how to register an input action
	// which would also allow rebinding of them, but this will work for now
	INPUTACTION_CAMERA_UP = INPUTACTION_TRIANGLE,
	INPUTACTION_CAMERA_DOWN = INPUTACTION_SQUARE,
	INPUTACTION_CAMERA_FAST = INPUTACTION_CIRCLE,
	INPUTACTION_CAMERA_SLOW = INPUTACTION_R2,
#else
	INPUTACTION_TRIANGLE	= 11,
	INPUTACTION_SQUARE		= 12,
	INPUTACTION_CIRCLE		= 13,
	INPUTACTION_CROSS		= 14,
	INPUTACTION_L1			= 15,
	INPUTACTION_R1			= 16,
	INPUTACTION_L2			= 17,
	INPUTACTION_R2			= 18,

	INPUTACTION_CAMERA_UP	= INPUTACTION_TRIANGLE,
	INPUTACTION_CAMERA_DOWN = INPUTACTION_SQUARE,
	INPUTACTION_CAMERA_FAST = INPUTACTION_L2,
	INPUTACTION_CAMERA_SLOW = INPUTACTION_CIRCLE,
#endif
};

enum INPUTAXIS
{
	INPUTAXIS_LANALOGUEX,
	INPUTAXIS_LANALOGUEY,
	INPUTAXIS_RANALOGUEX,
	INPUTAXIS_RANALOGUEY,

	INPUTAXIS_MOUSEX = 16,
	INPUTAXIS_MOUSEY = 17,
};

class InputActionMapper
{
public:
	struct ActionResult
	{
		unsigned int state;

#ifndef TR8
		char pad1[20];
#else
		char pad1[44];
#endif
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
	float GetAxisValue(int axisID) const noexcept;
};

class Input
{
public:
	static void DisableInput(bool disable) noexcept;
	static void DisablePlayerControl(bool disable) noexcept;
	static bool IsPlayerControlEnabled() noexcept;
	static bool IsInputActionPressed(int action) noexcept;

	static InputSystem* GetInputSystem() noexcept;
};