#include <imgui.h>

#include "MainMenu.h"
#include "level/Stream.h"
#include "game/Game.h"
#include "util/Hooking.h"
#include "render/Font.h"

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

	// Time
	if (ImGui::CollapsingHeader("Time"))
	{
		auto gameTracker = Game::GetGameTracker();

		ImGui::SliderFloat("Time multiplier", &gameTracker->timeDilation, 0.f, 10.f, "%.2f");
	}

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

void MainMenu::OnFrame()
{
	auto mainState = *(int*)GET_ADDRESS(0x10E5868, 0x838838, 0x000000);

	if (mainState == MS_DISPLAY_MAIN_MENU)
	{
		auto font = Font::GetMainFont();

		font->SetCursor(5.f, 430.f);
		font->Print("TRLAU-Menu-Hook");
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