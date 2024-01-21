#include <imgui.h>
#include <imgui_impl_dx9.h>
#include <imgui_impl_win32.h>

#include <MinHook.h>

#include "Menu.h"
#include "render/RenderContext.h"
#include "input/MouseHook.h"
#include "input/Input.h"
#include "Hook.h"

#include "cdc/render/PCDeviceManager.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

Menu::Menu() : PCInternalResource()
{
	ImGui::CreateContext();

	// We can do this since we always create the menu after D3D_Init
	ImGui_ImplWin32_Init(cdc::PCDeviceManager::s_pInstance->GetWindow());

	// Add present callback to draw the UI
	RenderContext::OnPresent(std::bind(&Menu::OnPresent, this));
	MouseHook::Init();

	OnConstruct();
}

bool Menu::OnCreateDevice()
{
	// Initialize the DirectX9 backend for ImGui
	ImGui_ImplDX9_Init(cdc::PCDeviceManager::s_pInstance->GetD3DDevice());

	m_initialized = true;

	return true;
}

void Menu::OnDestroyDevice()
{
	if (m_initialized)
	{
		// We need to destroy the full backend since the game destroys the full device
		// not only call reset
		ImGui_ImplDX9_Shutdown();

		m_initialized = false;
	}
}

void Menu::OnPresent()
{
	// Set the mouse cursor visible when we have focus
	ImGui::GetIO().MouseDrawCursor = m_focus;

	// Start the frame
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();

	ImGui::NewFrame();

	// Draw the menu
	if (m_focus)
	{
		Draw();
	}

	// End the frame
	ImGui::EndFrame();

	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

void Menu::OnMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_KEYUP && wParam == VK_F8)
	{
		SetFocus(!m_focus);
	}

	if (m_focus)
	{
		ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
	}
}

void Menu::Draw()
{
	auto& modules = Hook::GetInstance().GetModules();

	if (ImGui::BeginMainMenuBar())
	{
		// Draw all modules menus
		for (auto& [hash, mod] : modules)
		{
			mod->OnMenu();
		}

		ImGui::EndMainMenuBar();
	}

	// Draw all menus
	for (auto& [hash, mod] : modules)
	{
		mod->OnDraw();
	}
}

void Menu::SetFocus(bool focus)
{
	m_focus = focus;

	// Disable the cursor lock and game input
	MouseHook::DisableCursorLock(focus);
	Input::DisableInput(focus);
}

bool Menu::HasFocus()
{
	return m_focus;
}