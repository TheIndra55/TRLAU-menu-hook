#pragma once

#include "modules/Module.h"

// Base class for both free camera implementations
class FreeCameraBase : public Module
{
public:
	enum ControlMode
	{
		Disabled,
		Enabled,
		NoControl
	};

private:
	bool m_vKeys[3];

protected:
	ControlMode m_mode = Disabled;

	virtual void ToggleMode();
	virtual void OnControl();

	// Game specific
	virtual void Rotate(float x, float z) = 0;
	virtual void Rotate(float y) = 0;
	virtual void MoveForward(float distance) = 0;
	virtual void MoveLeft(float distance) = 0;
	virtual void MoveUp(float distance) = 0;

public:
	FreeCameraBase();

	void OnInput(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	void OnLoop();
};

// Free camera implementation for Legend and Anniversary
class LegendCamera : public FreeCameraBase
{
protected:
	void ToggleMode();

	void Rotate(float x, float z);
	void Rotate(float y);
	void MoveForward(float distance);
	void MoveLeft(float distance);
	void MoveUp(float distance);
};

using FreeCamera = LegendCamera;