#include "Menu.hpp"
#include "Game.hpp"
#include "Hooking.hpp"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static bool shouldInstance = true;
static bool shouldReloc = true;

Instance*(__cdecl* INSTANCE_NewInstance)();

Instance* newinstance()
{
    if (shouldInstance)
        return INSTANCE_NewInstance();

    return 0;
}

extern Hooking* g_hooking;

// most of this code is written in 2 minutes to confirm finds
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

char IsPs2()
{
    // somewhat the zoom code for binoculars is inside an 'if IsPs2()' in TRAE so we have to hook that function
    return Game::m_binoculars;
}

bool isDiskFS = false;
bool switchPlayerNextFrame = false;

extern int(__thiscall* MSFileSystem_FileExists)(int _this, const char* file);

int(__cdecl* origIMAGE_LoadImage)(char* name);
int __cdecl IMAGE_LoadImage(char* name)
{
    char string[256];
    sprintf_s(string, "\\units\\%s.raw", name);

    if (MSFileSystem_FileExists(*(int*)DISKFS, string))
    {
        g_hooking->menu->Log("%s exists, loading that one instead\n", string);
        isDiskFS = true;
    }

    auto ret = origIMAGE_LoadImage(name);
    isDiskFS = false;

    return ret;
}

int(__cdecl* origInsertGlobalObject)(int a1);
int __cdecl InsertGlobalObject(int a1)
{
#if TRAE
    auto objects = *(int*)0x842C70;
#elif TR7
    auto objects = *(int*)0x10F9110;
#elif TR8
    auto objects = 0;
#endif
    auto name = (char*)*(int*)(objects + 8 * a1 - 4);

    char string[256];
    sprintf_s(string, "\\units\\%s.drm", name);

    if (MSFileSystem_FileExists(*(int*)DISKFS, string))
    {
        g_hooking->menu->Log("%s exists, loading that one instead\n", string);
        isDiskFS = true;
    }

    auto ret = origInsertGlobalObject(a1);
    isDiskFS = false;

    return ret;
}

void(__cdecl* origUnitFileName)(char*, char*, char*);
void unitFileName(char* name, char* unit, char* ext)
{
    if (isDiskFS)
    {
        sprintf(name, "\\units\\%s.drm", unit);
        return;
    }

    origUnitFileName(name, unit, ext);
}

void(__cdecl* origImageFileName)(char*, char*);
void __cdecl imageFileName(char* name, char* image)
{
    if (isDiskFS)
    {
        sprintf(name, "\\units\\%s.raw", image);
        return;
    }

    origImageFileName(name, image);
}

int getFS()
{
    if (isDiskFS)
    {
        return *(int*)DISKFS;
    }

#if TRAE
    return *(int*)0x83888C;
#elif TR8
    return *(int*)0x9CE278;
#elif TR7
    return *(int*)0x10EEC7C;
#endif
}

int(__cdecl* origSTREAM_LoadLevel)(char* a1, int a2, char a3);

int __cdecl STREAM_LoadLevel(char* a1, int a2, char a3)
{
    g_hooking->menu->Log(__FUNCTION__ " Loading %s\n", a1);

    if (strncmp("fi", a1, 2) == 0)
    {
        // load this unit from disk
        isDiskFS = true;
    }

    auto unit = origSTREAM_LoadLevel(a1, a2, a3);
    isDiskFS = false;

    return unit;
}

void(__cdecl* origSTREAM_FinishLoad)(StreamUnit* unit);
void STREAM_FinishLoad(StreamUnit* unit)
{
    if (!shouldReloc)
    {
        Level* level = unit->level;
        *(DWORD*)(level + 156) = 0; // set reloc module ptr to 0
    }

    origSTREAM_FinishLoad(unit);
}

Menu::Menu(LPDIRECT3DDEVICE9 pd3dDevice, HWND hwnd)
{
	m_pd3dDevice = pd3dDevice;
	m_hwnd = hwnd;

	ImGui::CreateContext();

	ImGui_ImplWin32_Init(m_hwnd);
	ImGui_ImplDX9_Init(m_pd3dDevice);

#if TRAE
    MH_CreateHook((void*)0x00C62479, newinstance, (void**)&INSTANCE_NewInstance);
    MH_CreateHook((void*)0x4FCB60, pushscreenhooked, (void**)&pushscreen);
    MH_CreateHook((void*)0x0046F080, hooked_Subtitle_Add, (void**)&orginal_Subtitle_Add);

    MH_CreateHook((void*)0x004E6EC0, IsPs2, nullptr);
    
    MH_CreateHook((void*)0x00C7DC5B, STREAM_LoadLevel, (void**)&origSTREAM_LoadLevel);
    MH_CreateHook((void*)0x005DB680, STREAM_FinishLoad, (void**)&origSTREAM_FinishLoad);
#endif

#if TRAE
    MH_CreateHook((void*)0x0045F640, getFS, nullptr);
    MH_CreateHook((void*)0x00C63241, unitFileName, (void**)&origUnitFileName);

    MH_CreateHook((void*)0x00C7D980, InsertGlobalObject, (void**)&origInsertGlobalObject);
    MSFileSystem_FileExists = reinterpret_cast<int(__thiscall*)(int _this, const char* file)>(0x005E52C0);

    MH_CreateHook((void*)0x00401480, IMAGE_LoadImage, (void**)&origIMAGE_LoadImage);
    MH_CreateHook((void*)0x00C63280, imageFileName, (void**)&origImageFileName);
#elif TR7
    MH_CreateHook((void*)0x0045F420, getFS, nullptr);
    MH_CreateHook((void*)0x0045F4D0, unitFileName, (void**)&origUnitFileName);

    MH_CreateHook((void*)0x005DB550, InsertGlobalObject, (void**)&origInsertGlobalObject);
    MSFileSystem_FileExists = reinterpret_cast<int(__thiscall*)(int _this, const char* file)>(0x0047DC70);

    MH_CreateHook((void*)0x00401480, IMAGE_LoadImage, (void**)&origIMAGE_LoadImage);
    MH_CreateHook((void*)0x0045F520, imageFileName, (void**)&origImageFileName);
#endif

#if TR8
    MH_CreateHook((void*)0x00472B50, getFS, nullptr);
    MH_CreateHook((void*)0x00477970, unitFileName, (void**)&origUnitFileName);
    MH_CreateHook((void*)0x005D23F0, STREAM_LoadLevel, (void**)&origSTREAM_LoadLevel);
#endif
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

    if (m_visible)
    {
        this->Draw();
    }

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

    if (msg == WM_KEYUP && wparam == VK_F7)
    {
        m_visible = !m_visible;
    }

#if TRAE
    if (msg == WM_KEYUP && wparam == VK_DELETE)
    {
        Game::ToggleBinoculars();
    }

    if (msg == WM_KEYUP && wparam == VK_F3)
    {
        auto streamFlags = (int*)0x8383F4;
        if (*streamFlags & 0x1000)
        {
            *streamFlags &= 0xFFFFEFFF;
        }
        else
        {
            *streamFlags |= 0x1000u;
        }
    }

    if (msg == WM_KEYUP && wparam == VK_F4)
    {
        auto cameraMode = (int*)0x850984;
        *cameraMode = *cameraMode == 7 ? 2 : 7;
    }

    if (switchPlayerNextFrame)
    {
        switchPlayerNextFrame = false;

        Game::SwitchPlayerCharacter();
    }
#endif

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

#if TRAE || TR7
    auto ptr = reinterpret_cast<int*>(GAMETRACKER + 227);
#elif TR8
    auto ptr = reinterpret_cast<int*>(0xE7F143);
#endif

    if (m_flight)
    {
        //*ptr |= 256;
        *ptr |= 256;
    }
    else
    {
        //*ptr ^= 256;
        *ptr = 0;

        // drop player when flight disabled
#if TRAE || TR7
        Game::InstancePost(*reinterpret_cast<Instance**>(PLAYERINSTANCE), 1048592, 0);
#endif
    }
}

void Menu::ProcessFlight(UINT msg, WPARAM wparam)
{
    auto base = *reinterpret_cast<DWORD*>(PLAYERINSTANCE);

#if TRAE || TR7
    auto z = reinterpret_cast<float*>(base + 24);
#elif TR8
    auto z = reinterpret_cast<float*>(base + 40);
#endif

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

    static bool show_instance_viewer = false;
    if (show_instance_viewer) DrawInstanceViewer();

    ImGui::Begin("Menu", nullptr, ImGuiWindowFlags_MenuBar);

#if TRAE
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Tools"))
        {
            ImGui::MenuItem("Instance viewer", NULL, &show_instance_viewer);
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    // show current unit
    auto streamUnit = (int)(*(DWORD*)0x83833C) + 178;
#endif
    ImGui::Text("F2 = Flight, F8 = Toggle menu focus, F9 = Switch player character");
    ImGui::Text("Unit = %s, Flight = %s", (char*)GAMETRACKER_BASE_AREA, m_flight ? "true" : "false");

    ImGui::SliderFloat("Z speed", &m_flightSpeed, 0, 500);
#if TRAE

    if (ImGui::Button("Fill 'er Up"))
    {
        // pointers everywhere!
        auto missingHealth = *(float*)(*(DWORD*)((*(DWORD*)PLAYERINSTANCE) + 448) + 164);

        Game::IncreaseHealth(missingHealth);
        Game::TriggerUiFadeGroup(1);
        // TODO ammo
    }

    ImGui::Checkbox("Should instance?", &shouldInstance);
    ImGui::Checkbox("Load unit script", &shouldReloc);
    ImGui::Checkbox("Enable debug keypad", (bool*)0x7C8A3C);

    if (ImGui::Checkbox("Enable debug draw", &m_drawSettings.drawDebug))
    {
        *(int*)0x838348/*debugFlags2*/ |= 0x100;
    }

    ImGui::Checkbox("Wireframe", (bool*)0x7C7CD4);
    ImGui::Checkbox("Draw instances", &m_drawSettings.draw);
    if (ImGui::CollapsingHeader("Draw settings"))
    {
        ImGui::Checkbox("Filter", &m_drawSettings.filter);
        ImGui::Checkbox("Draw intro id", &m_drawSettings.drawIntro);
        ImGui::Checkbox("Draw pointer address", &m_drawSettings.drawAddress);
        ImGui::Checkbox("Draw 'family'", &m_drawSettings.drawFamily);
        ImGui::Checkbox("Draw enemy health", &m_drawSettings.drawHealth);
    }

    ImGui::InputText("chapter", chapter, 32);
#endif
    ImGui::InputText("unit", unit, MAX_UNIT_LEN);

#if TRAE
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
#endif
    if (ImGui::Button("Load unit"))
    {
        // change current unit
        strcpy_s((char*)GAMETRACKER_BASE_AREA, MAX_UNIT_LEN, unit);
        Game::ResetGame(4);
    }

#if TRAE
    ImGui::Text("screen: %d", Game::GetTopScreenID());

    static char screenId[8] = "";
    ImGui::InputText("screen Id", screenId, 8);
    if (ImGui::Button("Push screen"))
    {
        int screen = std::atoi(screenId);

        Game::PushScreen(screen, 0);
    }
#endif


    if (ImGui::Button("Clear")) {
        this->logBuffer.clear();
    }

    if (ImGui::Button("List units"))
    {
#if TRAE
        auto unitList = *(DWORD*)0x8AF44C;
#elif TR8
        auto unitList = *(DWORD*)0xDBA188;
#elif TR7
        auto unitList = *(DWORD*)0x1162C54;
#endif
        auto numUnits = *(int*)unitList;
        Log("numUnits: %d\n", numUnits);

        int offset = 0;
        int count = 0;
        while (true)
        {
            Log("%s\n", (char*)(unitList + 4 + offset));

#if TRAE || TR7
            offset += 20;
#elif TR8
            offset += 132;
#endif
            count++;
            if (count >= numUnits) break;
        }

        ImGui::SetClipboardText(this->logBuffer.begin());
    }

#if TRAE || TR7
    if (ImGui::Button("List instances"))
    {
#if TRAE
        auto instance = *(DWORD*)0x817D64;
#elif TR7
        auto instance = *(DWORD*)0x10CEE64;
#endif
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
#endif

#if TRAE
    if (ImGui::Button("Trigger All Fade Groups"))
    {
        for (int i = 0; i <= 28; i++)
        {
            Game::TriggerUiFadeGroup(i);
        }

        Game::SetGameValue(126, 10.0, 1);
    }

    if (ImGui::Button("Turn Lara into gold anim"))
    {
        auto player = *reinterpret_cast<Instance**>(PLAYERINSTANCE);
        int v11 = 52;

        Game::InstancePost(player, 262158, 1);

        auto anim = Game::AnimDataSomething((int)player, (int)player, (int)&v11);
        Game::InstancePost(player, 262147, anim);
        Game::InstanceSetEventAnimPlaying(player, 0);
        Game::PlayerTurnGold();
    }

    if (ImGui::Button("Real gold lara"))
    {
        Game::PlayerTurnGold();
    }

    auto player = *reinterpret_cast<Instance**>(PLAYERINSTANCE);
    if (ImGui::Button("Give all weapons"))
    {
        for (int i = 0; i < 3; i++)
        {
            Game::InstancePost(player, 262256, i);
        }
    }

    static int item;
    ImGui::InputInt("itemId", &item);
    if (ImGui::Button("Give item"))
    {
        Game::InstancePost(player, 262256, item );
    }
#endif

#if TRAE || TR7
    static char name[100] = "";
    ImGui::InputText("name", name, 100);
    if (ImGui::Button("Birth instance"))
    {
        auto position = (*(Instance**)PLAYERINSTANCE)->position;
        auto rotation = (*(Instance**)PLAYERINSTANCE)->rotation;
#if TRAE
        auto unitId = *(int*)0x838418;
#elif TR7
        auto unitId = *(int*)(GAMETRACKER + 0xE8);
#endif

        auto tracker = Stream::GetObjectTrackerByName(name);
        while (tracker->status != 2 && Stream::PollLoadQueue());

        Game::BirthObjectNoParent(unitId, &position, &rotation, nullptr, tracker->object, 0, 1);
    }
#endif

#if TRAE
    static char outfit[100] = "";
    ImGui::InputText("outfit", outfit, 100);
    if (ImGui::Button("Load outfit"))
    {
        switchPlayerNextFrame = true;

        auto obj = Game::GetObjectID(outfit);
        *(int*)0x838768 /* alt player object */ = obj;
    }
#endif

#if TR8
    // if my research is right these pointers enable (or were supposed to) some screenshot feature
    // this might be the cut photo feature, though right not it doesn't seem to change much
    ImGui::Checkbox("ui", (bool*)0xFAE80C);
    ImGui::Checkbox("screenshots", (bool*)0xFAE80D);
#endif

    ImGui::End();

    ImGui::Begin("Log", nullptr);

    // log window (based on imgui_demo log window)
    ImGui::BeginChild("LogRegion", ImVec2(), true);

    ImGui::TextUnformatted(this->logBuffer.begin());
    ImGui::SetScrollHere(1.0f);
    ImGui::EndChild();

    ImGui::End();
}

void DrawInstanceViewer()
{
    static DWORD clickedInstance;

    ImGui::Begin("Instances");

    ImGui::Columns(2, "instances");

    auto instance = *(DWORD*)0x817D64;

    ImGui::BeginChild("InstanceListTree");
    if (instance)
    {
        while (1)
        {
            auto next = *(DWORD*)(instance + 8);
            auto object = *(DWORD*)(instance + 0x94);

            if (ImGui::TreeNodeEx((void*)object, ImGuiTreeNodeFlags_Leaf, "%d %s", *(int*)(instance + 0x1D0), (char*)*(DWORD*)(object + 0x48)))
            {
                if (ImGui::IsItemClicked())
                {
                    clickedInstance = instance;
                }

                ImGui::TreePop();
            }

            if (!next)
                break;

            instance = next;
        }
    }
    ImGui::EndChild();

    ImGui::NextColumn();

    if (clickedInstance && *(int*)clickedInstance == 0)
    {
        // points to nullptr remove
        clickedInstance = 0;
    }

    if (clickedInstance)
    {
        auto oInstance = (Instance*)clickedInstance;
        auto object = *(DWORD*)(clickedInstance + 0x94);
        auto data = *(DWORD*)(clickedInstance + 448);
        auto extraData = *(DWORD*)(clickedInstance + 572);

        ImGui::Text("%s", (char*)*(DWORD*)(object + 0x48));

        auto coords = oInstance->position;
        auto rotation = oInstance->rotation;

        ImGui::Text("Position: %f %f %f", coords.x, coords.y, coords.z);
        ImGui::Text("Rotation: %f %f %f", rotation.x, rotation.y, rotation.z);

        // pretty sure ImGui has some sort of widget for this
        if (ImGui::Button("X")) { rotation.x += 0.1f; } ImGui::SameLine();
        if (ImGui::Button("Y")) { rotation.y += 0.1f; } ImGui::SameLine();
        if (ImGui::Button("Z")) { rotation.z += 0.1f; }
        oInstance->rotation = rotation;

        ImGui::Text("Intro: %d", *(int*)(clickedInstance + 0x1D0));

        if (data)
        {
            ImGui::Text("Family %d", *(unsigned __int16*)(data + 2));
        }

        if (extraData)
        {
            ImGui::Text("Health: %8.2f", *(float*)(extraData + 5280));
        }

        if (ImGui::Button("Switch down"))
        {
            Game::InstancePost((Instance*)clickedInstance, 8388753, 1);
        }
        if (ImGui::Button("Switch up"))
        {
            Game::InstancePost((Instance*)clickedInstance, 8388753, 2);
        }
        ImGui::Text("Switch status: %d", Game::InstanceQuery((Instance*)clickedInstance, 233));
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