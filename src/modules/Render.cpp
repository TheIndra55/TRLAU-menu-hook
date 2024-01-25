#include <imgui.h>

#include "Render.h"
#include "util/Hooking.h"

void Render::OnMenu()
{
	if (ImGui::BeginMenu("Render"))
	{
		// TODO Wireframe for Underworld
		ImGui::MenuItem("Wireframe", nullptr, (bool*)GET_ADDRESS(0x1075BD4, 0x7C7CD4, 0x000000));

		ImGui::EndMenu();
	}
}