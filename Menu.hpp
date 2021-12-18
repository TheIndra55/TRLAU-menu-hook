#pragma once
#include <Windows.h>
#include <d3d9.h>

#include "imgui.h"
#include "vendor/imgui/examples/imgui_impl_dx9.h"
#include "vendor/imgui/examples/imgui_impl_win32.h"

#include "Game.hpp"

struct DrawSettings
{
	bool draw = false;
	bool filter = true;
	bool drawIntro = false;
	bool drawAddress = false;
	bool drawFamily = false;
	bool drawHealth = false;
	bool drawAnim = false;

	bool drawDebug = false;
	bool drawMarkup = false;
	bool drawPath = false;
	bool drawCollision = false;
	bool drawPortals = false;
	bool drawSignals = false;

	// "DrawSettings"
	bool noRespawn = false;
	bool noMovieBars = false;
};

class Menu
{
public:
	Menu(LPDIRECT3DDEVICE9 pd3dDevice, HWND hwnd);

	void OnDeviceReleased();
	void OnPresent();
	void Process(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	void ToggleFlight(bool flight);
	void SetDevice(LPDIRECT3DDEVICE9 pd3dDevice);
	void Log(const char* fmt, ...);

	bool IsFreecam() const noexcept;
	bool IsFocus() const noexcept;
	bool IsVisible() const noexcept;
	void SetFocus(bool value) noexcept;
	void SetVisibility(bool value) noexcept;

	void LoadSettings() noexcept;
	void SaveSettings() const noexcept;

	Instance* GetClickedInstance() noexcept;

	DrawSettings m_drawSettings;
private:
	void Draw();
	void ProcessFlight(UINT msg, WPARAM wparam);
	void DrawInstanceViewer(bool* show_instance_viewer);
	void DrawEventDebugViewer(bool* show_event_debug_viewer) const noexcept;

	void OnLayoutChange() noexcept;

private:
	bool m_focus = false;
	bool m_flight = false;
	float m_flightSpeed = 75.f;

	bool m_visible = true;
	bool m_isAzertyLayout = false;

	bool m_freecam = false;

	ImGuiTextBuffer logBuffer;

	LPDIRECT3DDEVICE9 m_pd3dDevice;
	HWND m_hwnd;

	DWORD clickedInstance = 0;
};

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);