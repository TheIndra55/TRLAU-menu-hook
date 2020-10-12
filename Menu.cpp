#include "Menu.hpp"
#include "Game.hpp"
#include "Hooking.hpp"

#define GAMETRACKER 0x838330
#define PLAYERINSTANCE 0x83833C

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static bool shouldInstance = true;

DWORD(__cdecl*trampinstance)();

DWORD newinstance()
{
    if (shouldInstance)
        return trampinstance();

    return 0;
}

Menu::Menu(LPDIRECT3DDEVICE9 pd3dDevice, HWND hwnd)
{
	m_pd3dDevice = pd3dDevice;
	m_hwnd = hwnd;

	ImGui::CreateContext();

	ImGui_ImplWin32_Init(m_hwnd);
	ImGui_ImplDX9_Init(m_pd3dDevice);

    MH_CreateHook(reinterpret_cast<void*>(0x46BF90), hooked_SIGNAL_FindSignal, reinterpret_cast<void**>(&original_SIGNAL_FindSignal));

    MH_CreateHook((void*)0x00C62479, newinstance, (void**)&trampinstance);
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
    if (msg == WM_KEYUP && wparam == VK_F2)
    {
        ToggleFlight(!m_flight);
    }

    if (m_flight)
    {
        ProcessFlight(msg, wparam);
    }

	if (m_focus)
	{
		ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam);
	}
}

void Menu::ToggleFlight(bool flight)
{
    m_flight = flight;
    *reinterpret_cast<int*>(GAMETRACKER + 227) = m_flight ? 256 : 0;

    if (!m_flight)
    {
        // drop player when flight disabled
        Game::InstancePost(*reinterpret_cast<DWORD*>(PLAYERINSTANCE), 1048592, 0);
    }
}

void Menu::ProcessFlight(UINT msg, WPARAM wparam)
{
    auto base = *reinterpret_cast<DWORD*>(0x83833C);
    auto z = reinterpret_cast<float*>(base + 24);

    // TODO azerty?
    if (msg == WM_KEYDOWN && wparam == 0x51/*Q Key*/)
    {
        *z += (*z + 0.5);
    }

    if (msg == WM_KEYDOWN && wparam == 0x5A/*Z Key*/)
    {
        *z -= (*z + 0.5);
    }
}

// used for PushOkDialog so it pops the dialog away after pressing OK
void __cdecl DialogFn()
{
    Game::PopScreen();
}

void(__cdecl* EVENT_MoveSunPosition)(int, int, int);

void Menu::Draw()
{
    static char chapter[32] = "";
    static char unit[32] = "";

    ImGui::Begin("Menu", nullptr);

    // show current unit
    auto streamUnit = (int)(*(DWORD*)0x83833C) + 178;
    ImGui::Text("Unit = %s, Flight = %s", (char*)(GAMETRACKER + 204), m_flight ? "true" : "false");
    ImGui::Text("currentStreamUnitID = %d", streamUnit);

    ImGui::Checkbox("should instance?", &shouldInstance);

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

    // log window (based on imgui_demo log window)
    ImGui::BeginChild("LogRegion", ImVec2(), true);

    ImGui::TextUnformatted(this->logBuffer.begin());
    ImGui::SetScrollHere(1.0f);
    ImGui::EndChild();

    if (ImGui::Button("Clear")) {
        this->logBuffer.clear();
    }

    if (ImGui::Button("List units"))
    {
        auto unitList = *(DWORD*)0x8AF44C;
        auto numUnits = *(int*)unitList;
        Log("numUnits: %d\n", numUnits);

        int offset = 0;
        int count = 0;
        while (true)
        {
            Log("%s\n", (char*)(unitList + 4 + offset));

            offset += 20;
            count++;
            if (count >= numUnits) break;
        }

        ImGui::SetClipboardText(this->logBuffer.begin());
    }

    if (ImGui::Button("List instances"))
    {
        auto instance = *(DWORD*)0x817D64;
        while (1)
        {
            auto next = *(DWORD*)(instance + 8);
            auto object = *(DWORD*)(instance + 0x94);
            Log("%s - %d\n", (char*)*(DWORD*)(object + 0x48), *(int*)(instance + 0x1D0));

            if (!next)
                break;

            instance = next;
        }

        ImGui::SetClipboardText(this->logBuffer.begin());
    }

    static int rot;
    static int angle;
    static int frames;
    ImGui::InputInt("rot", &rot);
    ImGui::InputInt("angle", &angle);
    ImGui::InputInt("frames", &frames);

    if (ImGui::Button("set sun")) {
        EVENT_MoveSunPosition = (void(__cdecl*)(int, int, int))0x42F8A0;
        EVENT_MoveSunPosition(rot, angle, frames);
    }

    ImGui::End();
}

void Menu::Log(const char* fmt, ...) IM_FMTARGS(2)
{
    va_list args;
    va_start(args, fmt);
    this->logBuffer.appendfv(fmt, args);
    va_end(args);
}

extern Hooking* g_hooking;

int hooked_SIGNAL_FindSignal(DWORD* level, int signal)
{
    g_hooking->menu->Log("signal %d for unit %d\n", signal, *reinterpret_cast<int*>(level + 180));

    return original_SIGNAL_FindSignal(level, signal);
}