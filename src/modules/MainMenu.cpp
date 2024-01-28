#include <imgui.h>

#include "MainMenu.h"
#include "level/Stream.h"
#include "game/Game.h"
#include "util/Hooking.h"
#include "render/Font.h"
#include "level/Event.h"
#include "input/Input.h"

void MainMenu::OnDraw()
{
	ImGui::Begin("Menu");

	// Unit select
	static char unit[MAX_UNIT_NAME_LENGTH] = "";
	ImGui::InputText("Unit", unit, sizeof(unit));

	if (ImGui::Button("Load unit"))
	{
		GAMELOOP_RequestLevelChangeByName(unit, Game::GetGameTracker(), 4);
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

	// Save
#ifndef TR8
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

	ImGui::End();
}

void MainMenu::BirthObject(char* name)
{
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

void MainMenu::SwitchPlayerCharacter(char* name)
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
	// Shows the watermark in th main menu
	auto mainState = *(int*)GET_ADDRESS(0x10E5868, 0x838838, 0x000000);

	if (mainState == MS_DISPLAY_MAIN_MENU)
	{
		auto font = Font::GetMainFont();

		font->SetCursor(5.f, 430.f);
		font->Print("TRLAU-Menu-Hook");
	}
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