#include <imgui.h>
#include <imgui_impl_dx9.h>
#include <imgui_impl_win32.h>

#include <MinHook.h>

#include "Menu.h"
#include "render/RenderContext.h"
#include "input/MouseHook.h"
#include "input/Input.h"
#include "Hook.h"
#include "Options.h"

#include "cdc/render/PCDeviceManager.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

Menu::Menu() : PCInternalResource()
{
	ImGui::CreateContext();

	// We can do this since we always create the menu after D3D_Init
	ImGui_ImplWin32_Init(cdc::PCDeviceManager::GetInstance()->GetWindow());

	// Add present callback to draw the UI
	RenderContext::OnPresent(std::bind(&Menu::OnPresent, this));
	MouseHook::Init();

	OnConstruct();
}

bool Menu::OnCreateDevice()
{
	// Initialize the DirectX9 backend for ImGui
	ImGui_ImplDX9_Init(cdc::PCDeviceManager::GetInstance()->GetD3DDevice());

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
	if (m_visible)
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
		SetFocus(!m_focus, ImGui::IsKeyDown(ImGuiKey_LeftShift));
	}

	if (m_focus)
	{
		ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
	}
}

void Menu::Draw() noexcept
{
	auto& modules = Hook::GetInstance().GetModules();

	if (m_focus && ImGui::BeginMainMenuBar())
	{
		// Draw all modules menus
		for (auto& [hash, mod] : modules)
		{
			mod->OnMenu();
		}

		// Draw last menu
		DrawMenu();

		ImGui::EndMainMenuBar();
	}

	// Draw all menus
	for (auto& [hash, mod] : modules)
	{
		mod->OnDraw();
	}
}

void Menu::DrawMenu() noexcept
{
	if (ImGui::BeginMenu("Help"))
	{
		if (ImGui::MenuItem("Options"))
		{
			Hook::GetInstance().GetModule<Options>()->SetVisible(true);
		}

		if (ImGui::MenuItem("GitHub"))
		{
			ShellExecuteA(NULL, NULL, "https://github.com/TheIndra55/TRAE-menu-hook", NULL, NULL, SW_SHOW);
		}

		ImGui::EndMenu();
	}
}

void Menu::SetFocus(bool focus, bool dontHide) noexcept
{
	m_focus = focus;
	m_visible = focus;

	if (!focus && dontHide)
	{
		m_visible = true;
	}

	// Disable the cursor lock and game input
	MouseHook::DisableCursorLock(focus);
	Input::DisableInput(focus);
}

bool Menu::HasFocus() const noexcept
{
	return m_focus;
}