#include <imgui.h>

#include "MainMenu.h"
#include "level/Stream.h"
#include "game/Game.h"
#include "util/Hooking.h"
#include "render/Font.h"
#include "level/Event.h"
#include "input/Input.h"
#include "game/Player.h"
#include "file/FileSystem.h"
#include "modules/Log.h"
#include "game/Camera.h"
#include "util/Controls.h"

void MainMenu::OnDraw()
{
	ImGui::Begin("Menu");

	// Unit select
	static char unit[MAX_UNIT_NAME_LENGTH] = "";
	ImGui::InputText("Unit", unit, sizeof(unit));

	if (ImGui::Button("Load unit"))
	{
		SwitchUnit(unit);
	}

	// Birth instance
	static char object[64] = "";
	ImGui::InputText("Object", object, sizeof(object));

	if (ImGui::Button("Birth object"))
	{
		BirthObject(object);
	}

	// Player
	if (ImGui::CollapsingHeader("Player"))
	{
#ifndef TR8
		// Fill 'er up
		if (ImGui::Button("Fill 'er up"))
		{
			auto player = Game::GetPlayerInstance();
			auto data = (PlayerProp*)player->data;

			// TODO fill up ammo
			IncrHealth(data->oldData.HealthInitial);
		}

		// Switch outfit
		static char outfit[64] = "";
		ImGui::InputText("Outfit", outfit, sizeof(outfit));

		if (ImGui::Button("Change"))
		{
			SwitchPlayerCharacter(outfit);
		}

		ImGui::SameLine();

		if (ImGui::Button("Next"))
		{
			SwitchPlayerCharacter();
		}

		// Player flags
		auto flags = (unsigned int*)GET_ADDRESS(0x1075B88, 0x7C7C78, 0x000000);

		static bool noInterp = *flags & 0x80;
		if (ImGui::Checkbox("No interpolation", &noInterp))
		{
			if (noInterp)
			{
				*flags |= 0x80;
			}
			else
			{
				*flags &= ~0x80;
			}
		}
#endif

		ImGui::Checkbox("No death fade", &m_noDeathFade);
	}

	// Time
	if (ImGui::CollapsingHeader("Time"))
	{
		auto gameTracker = Game::GetGameTracker();

		ImGui::SliderFloat("Time multiplier", &gameTracker->timeDilation, 0.f, 10.f, "%.2f");

		if (ImGui::Button("Reset"))
		{
			gameTracker->timeDilation = 1.f;
		}
	}

#ifndef TR8
	// Save
	if (ImGui::CollapsingHeader("Save"))
	{
		static int var = 0;
		static int value = 0;

		ImGui::InputInt("Event variable", &var);
		ImGui::InputInt("Value", &value);

		if (ImGui::Button("Update"))
		{
			auto globalData = (GlobalData*)GET_ADDRESS(0x1076980, 0x7C8A50, 0x000000);

			globalData->eventVars[var] = value;
		}
	}
#endif

	// Camera
	if (ImGui::CollapsingHeader("Camera"))
	{
		auto camera = CAMERA_GetCamera();

#ifdef TR8
		ImGui::SliderFloat("Field of view", &camera->fov, 0.f, 3.f);
#else
		if (SliderProjection("Field of view", &camera->projDistance, 0.f, 3.f))
		{
			CAMERA_SetProjDistance(camera, camera->projDistance);
		}
#endif
	}

	ImGui::End();
}

void MainMenu::BirthObject(char* name) const noexcept
{
	// Make sure the object exists
	if (!CheckDataFile(name) || OBTABLE_GetObjectID(name) == 0)
	{
		Hook::GetInstance().GetModule<Log>()->WriteLine("Not birthing %s, object does not exists", name);
		return;
	}

	auto game = Game::GetGameTracker();
	auto player = Game::GetPlayerInstance();

	if (player == nullptr)
	{
		return;
	}

	// Load the object
	auto tracker = STREAM_GetObjectTrackerByName(name);
	while (tracker->objectStatus != 2 && STREAM_PollLoadQueue());

	// Birth the instance at the player position
	INSTANCE_BirthObjectNoParent(game->StreamUnitID, &player->position, &player->rotation, nullptr, tracker->object, 0, 1);
}

void MainMenu::SwitchUnit(char* name) const noexcept
{
	// Make sure the unit exists, this only checks if a file with the same name exists
	if (!CheckDataFile(name))
	{
		Hook::GetInstance().GetModule<Log>()->WriteLine("Not switching to unit %s, level does not exist", name);
		return;
	}

	GAMELOOP_RequestLevelChangeByName(name, Game::GetGameTracker(), 4);
}

void MainMenu::SwitchPlayerCharacter(char* name) noexcept
{
	auto game = Game::GetGameTracker();

	if (name)
	{
		auto object = OBTABLE_GetObjectID(name);
		game->altPlayerObjectID = object;
	}

	// We can't do this here since switching the player character waits a couple of frames
	// and we're in the middle of a frame
	m_switchPlayerNextFrame = true;
}

void MainMenu::OnFrame()
{
#ifndef TR8
	// Shows the watermark in th main menu
	auto mainState = *(int*)GET_ADDRESS(0x10E5868, 0x838838, 0xE7ED60);

	if (mainState == MS_DISPLAY_MAIN_MENU && !m_noWatermark.GetValue())
	{
		auto font = Font::GetMainFont();

		font->SetCursor(5.f, 430.f);
		font->Print("TRLAU-Menu-Hook");
	}
#endif
}

void MainMenu::OnLoop()
{
	if (m_switchPlayerNextFrame)
	{
		m_switchPlayerNextFrame = false;
		PLAYER_DebugSwitchPlayerCharacter();
	}
}

void MainMenu::OnInput(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	auto gameTracker = Game::GetGameTracker();

	// Freeze the game
	if (msg == WM_KEYUP && wParam == VK_F3)
	{
		gameTracker->streamFlags ^= 0x1000;
	}

	// Toggle player control
	if (msg == WM_KEYUP && wParam == VK_F5)
	{
		Input::DisablePlayerControl(Input::IsPlayerControlEnabled());
	}

	// Ragdoll death
	if (msg == WM_KEYUP && wParam == VK_F11)
	{
#ifndef TR8
		INSTANCE_Post(gameTracker->playerInstance, 262167, 3);
#else
		INSTANCE_Post(gameTracker->playerInstance, 4, 3);
#endif
	}
}

bool MainMenu::CheckDataFile(char* name) noexcept
{
	auto fileSystem = GetFS();

	char fileName[256];
	LOAD_UnitFileName(fileName, name, "drm");

	return fileSystem->FileExists(fileName);
}