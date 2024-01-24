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