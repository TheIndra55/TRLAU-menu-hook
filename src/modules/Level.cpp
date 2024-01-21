#include <imgui.h>
#include <MinHook.h>

#include "Level.h"
#include "level/Stream.h"
#include "level/Event.h"
#include "game/Game.h"
#include "util/Hooking.h"

static bool s_disableScript = false;
static void (*s_STREAM_FinishLoad)(StreamUnit*);

static void STREAM_FinishLoad(StreamUnit* streamUnit)
{
	if (s_disableScript)
	{
		streamUnit->level->relocModule = nullptr;
	}

	s_STREAM_FinishLoad(streamUnit);
}

LevelModule::LevelModule()
{
	// Insert hook
	auto addr = GET_ADDRESS(0x5D5640, 0x5DB680, 0x000000);

	MH_CreateHook((void*)addr, STREAM_FinishLoad, (void**)&s_STREAM_FinishLoad);
	MH_EnableHook(MH_ALL_HOOKS);
}

void LevelModule::OnMenu()
{
	if (ImGui::BeginMenu("Level"))
	{
		ImGui::MenuItem("Disable script", nullptr, &s_disableScript);
		ImGui::MenuItem("Event debug", nullptr, &m_eventDebug);

		ImGui::EndMenu();
	}
}

void LevelModule::OnDraw()
{
	if (m_eventDebug)
	{
		DrawEventDebug();
	}
}

void LevelModule::DrawEventDebug()
{
	ImGui::Begin("Event debug", &m_eventDebug);
	ImGui::Columns(2);

	auto stream = Game::GetStreamTracker();

	// Level list
	ImGui::BeginChild("LevelsTree");

	for (int i = 0; i < MAX_STREAM_UNITS; i++)
	{
		auto unit = &stream->StreamList[i];

		if (unit->used != 2)
		{
			continue;
		}

		if (ImGui::TreeNodeEx((void*)unit, ImGuiTreeNodeFlags_Leaf, "%s", unit->baseAreaName))
		{
			if (ImGui::IsItemClicked())
			{
				m_selected = unit;
			}

			ImGui::TreePop();
		}
	}

	ImGui::EndChild();

	// Event debug
	ImGui::NextColumn();

	if (m_selected && m_selected->used != 2)
	{
		m_selected = nullptr;
	}

	if (m_selected)
	{
		ImGui::BeginChild("EventDebug");

		DrawEventDebug(m_selected);

		ImGui::EndChild();
	}

	ImGui::End();
}

void LevelModule::DrawEventDebug(StreamUnit* unit)
{
	auto level = unit->level;

	if (!level->relocModule)
	{
		return;
	}

	// Get the event debug export
	auto eventVars = (EventVar*)RELOC_GetProcAddress(level->relocModule, "EventDebug");

	if (eventVars)
	{
		auto globalData = (GlobalData*)GET_ADDRESS(0x1076980, 0x7C8A50, 0x000000);
		auto i = 0;

		// Display all event variables
		while (true)
		{
			auto var = &eventVars[i++];

			if (var->name == nullptr)
				break;

			ImGui::Text("%d %s = %d", var->variable, var->name, globalData->eventVars[var->variable]);
		}

		// Display all unsaved variables
		auto unsavedVars = (UnsavedVar*)eventVars;
		while (true)
		{
			auto var = &unsavedVars[i++];

			if (var->name == nullptr || var->variable == nullptr)
				break;

			ImGui::Text("%s %d", var->name, *var->variable);
		}
	}
	else
	{
		ImGui::Text("No event debug for this level");
	}
}