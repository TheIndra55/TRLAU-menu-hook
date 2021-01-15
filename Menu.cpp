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

extern Hooking* g_hooking;

// most of this code is written in 2 minutes to confirm finds
char* (__cdecl* localstr_get)(int);
const char* localstr_get_hooked(int a1)
{
    return localstr_get(a1);
    //g_hooking->menu->Log("%d: %s\n", a1, str);

    //return "no";
}

char(__cdecl* pushscreen)(int, bool);

char pushscreenhooked(int screen, bool unk2)
{
    g_hooking->menu->Log("screen pushed: %d\n", screen);
    return pushscreen(screen, unk2);
}

void(__thiscall* orginal_Subtitle_Add)(DWORD*, char* str, int duration);
void __fastcall hooked_Subtitle_Add(DWORD* _this, void* _, char* str, int duration)
{
    g_hooking->menu->Log("%s %d\n", str, duration);

    orginal_Subtitle_Add(_this, str, duration);
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
    MH_CreateHook((void*)0x4E3C80, localstr_get_hooked, (void**)&localstr_get);
    MH_CreateHook((void*)0x4FCB60, pushscreenhooked, (void**)&pushscreen);
    MH_CreateHook((void*)0x0046F080, hooked_Subtitle_Add, (void**)&orginal_Subtitle_Add);
}

void Menu::OnDeviceReleased()
{
    m_pd3dDevice->Release();
}

void Menu::SetDevice(LPDIRECT3DDEVICE9 pd3dDevice)
{
    m_pd3dDevice = pd3dDevice;
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
    if (msg == WM_KEYUP && wparam == VK_F2)
    {
        ToggleFlight(!m_flight);
    }

    if (msg == WM_KEYUP && wparam == VK_F9)
    {
        Game::SwitchPlayerCharacter();
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
        *z += m_flightSpeed;
    }

    if (msg == WM_KEYDOWN && wparam == 0x5A/*Z Key*/)
    {
        *z -= m_flightSpeed;
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
    auto streamUnit = (int)(*(DWORD*)0x83833C) + 178;
    ImGui::Text("F2 = Flight, F8 = Toggle menu focus, F9 = Switch player character");
    ImGui::Text("Unit = %s, Flight = %s", (char*)(GAMETRACKER + 204), m_flight ? "true" : "false");

    ImGui::SliderFloat("Z speed", &m_flightSpeed, 0, 500);

    if (ImGui::Button("Fill 'er Up"))
    {
        // pointers everywhere!
        auto missingHealth = *(float*)(*(DWORD*)((*(DWORD*)PLAYERINSTANCE) + 448) + 164);

        Game::IncreaseHealth(missingHealth);
        Game::TriggerUiFadeGroup(1);
        // TODO ammo
    }

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

    if (ImGui::Button("Turn Lara into gold anim")) // TODO find out how to get gold effect
    {
        auto player = *reinterpret_cast<DWORD*>(PLAYERINSTANCE);
        int v11 = 52;

        Game::InstancePost(player, 262158, 1);

        auto anim = Game::AnimDataSomething(player, player, (int)&v11);
        Game::InstancePost(player, 262147, anim);
        Game::InstanceSetEventAnimPlaying(player, 0);
    }

    ImGui::End();

    ImGui::Begin("Log", nullptr);

    // log window (based on imgui_demo log window)
    ImGui::BeginChild("LogRegion", ImVec2(), true);

    ImGui::TextUnformatted(this->logBuffer.begin());
    ImGui::SetScrollHere(1.0f);
    ImGui::EndChild();

    ImGui::End();
}

void Menu::Log(const char* fmt, ...) IM_FMTARGS(2)
{
    va_list args;
    va_start(args, fmt);
    this->logBuffer.appendfv(fmt, args);
    va_end(args);
}

int hooked_SIGNAL_FindSignal(DWORD* level, int signal)
{
    //g_hooking->menu->Log("signal %d for unit %d\n", signal, *reinterpret_cast<int*>(level + 180));

    return original_SIGNAL_FindSignal(level, signal);
}