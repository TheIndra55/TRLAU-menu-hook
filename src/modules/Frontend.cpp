#include <imgui.h>

#include "Frontend.h"
#include "game/Game.h"

void Frontend::OnMenu()
{
	if (ImGui::BeginMenu("Frontend"))
	{
#ifndef TR8
		if (ImGui::MenuItem("Hide HUD", nullptr, &m_hideHud))
		{
			auto gameTracker = Game::GetGameTracker();

			if (m_hideHud)
			{
				gameTracker->debugFlags3 |= 0x8000;
			}
			else
			{
				gameTracker->debugFlags3 &= ~0x8000;
			}
		}
#else
		ImGui::MenuItem("Hide UI", nullptr, (bool*)0xCDC9A0);
#endif

		ImGui::EndMenu();
	}
}