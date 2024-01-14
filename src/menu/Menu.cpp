#include <imgui.h>
#include <imgui_impl_dx9.h>
#include <imgui_impl_win32.h>

#include "Menu.h"
#include "render/RenderContext.h"

#include "cdc/render/PCDeviceManager.h"

Menu::Menu() : PCInternalResource()
{
	ImGui::CreateContext();

	// We can do this since we always create the menu after D3D_Init
	ImGui_ImplWin32_Init(cdc::PCDeviceManager::s_pInstance->GetWindow());

	// Add present callback to draw the UI
	RenderContext::OnPresent([this] { OnPresent(); });

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
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();

	ImGui::NewFrame();

	// Draw the menu
	Draw();

	// End the frame
	ImGui::EndFrame();

	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

void Menu::Draw()
{
	ImGui::Begin("Menu");
	ImGui::Text("Hello, World!");
	ImGui::End();
}