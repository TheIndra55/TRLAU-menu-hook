#include "Menu.hpp"
#include "Game.hpp"

#define GAMETRACKER 0x838330

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

Menu::Menu(LPDIRECT3DDEVICE9 pd3dDevice, HWND hwnd)
{
	m_pd3dDevice = pd3dDevice;
	m_hwnd = hwnd;

	ImGui::CreateContext();

	ImGui_ImplWin32_Init(m_hwnd);
	ImGui_ImplDX9_Init(m_pd3dDevice);
}

void Menu::OnPresent()
{
	ImGui::GetIO().MouseDrawCursor = m_focus;

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();

	ImGui::NewFrame();
	// draw menu
	this->Draw();
	ImGui::EndFrame();

	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

void Menu::Process(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (m_focus)
	{
		ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam);
	}
}

// used for PushOkDialog so it pops the dialog away after pressing OK
void __cdecl DialogFn()
{
    Game::PopScreen();
}

void Menu::Draw()
{
    static char chapter[32] = "";
    static char unit[32] = "";

    ImGui::Begin("Menu", nullptr);

    // show current unit
    ImGui::Text("Unit: %s", (char*)(GAMETRACKER + 204));

    ImGui::InputText("chapter", chapter, 32);
    ImGui::InputText("unit", unit, 32);

    if (ImGui::Button("Load chapter"))
    {
        // load chapter (chapter0, chapter1..)
        // chapter0 = croft manor
        if(Game::CheckChapter(chapter))
        {
            Game::SwitchChapter(chapter);
        }
        else
        {
            Game::PushOkDialog("Chapter does not exist.", DialogFn, 0, 0);
        }
    }
    if (ImGui::Button("Load unit"))
    {
        // change current unit
        strcpy_s((char*)(GAMETRACKER + 204), 32, unit);
        Game::ResetGame(4);
    }

    ImGui::Text("screen: %d", Game::GetTopScreenID());

    static char screenId[8] = "";
    ImGui::InputText("screen Id", screenId, 8);
    if (ImGui::Button("Push screen"))
    {
        int screen = std::atoi(screenId);
        Game::PushScreen(screen, 0);
    }

    ImGui::End();
}