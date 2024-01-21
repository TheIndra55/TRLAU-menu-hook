#include <string>
#include <imgui.h>

#include "InstanceViewer.h"
#include "instance/Instances.h"
#include "instance/Animation.h"
#include "game/Game.h"

void InstanceViewer::OnMenu()
{
	if (ImGui::BeginMenu("Instance"))
	{
		ImGui::MenuItem("Instances", nullptr, &m_show);

		ImGui::EndMenu();
	}
}

void InstanceViewer::OnDraw()
{
	if (m_show)
	{
		ImGui::Begin("Instances", &m_show);
		ImGui::Columns(2, "instances");

		// Filter
		ImGui::InputText("Name", m_filter, sizeof(m_filter));

		// Instance list
		ImGui::BeginChild("InstancesTree");

		Instances::Iterate([this](Instance* instance)
			{
				auto name = instance->object->name;

				// Check filter
				if (strlen(m_filter) > 0 && strstr(name, m_filter) == 0)
				{
					return;
				}

				if (ImGui::TreeNodeEx((void*)instance, ImGuiTreeNodeFlags_Leaf, "%d %s", instance->introUniqueID, name))
				{
					if (ImGui::IsItemClicked())
					{
						m_selected = instance;
					}

					ImGui::TreePop();
				}
			});

		ImGui::EndChild();

		// Instance properties
		ImGui::NextColumn();

		// Check if the instance still exists
		if (m_selected && m_selected->node.prev == nullptr)
		{
			m_selected = nullptr;
		}

		if (m_selected)
		{
			DrawInstance();
		}

		ImGui::End();
	}
}

void InstanceViewer::DrawInstance()
{
	auto instance = m_selected;

	ImGui::Text("%s", instance->object->name);

	auto position = instance->position;
	auto rotation = instance->rotation;

	ImGui::Text("Intro: %d", instance->introUniqueID);
	ImGui::Text("Position: %f %f %f", position.x, position.y, position.z);
	ImGui::Text("Rotation: %f %f %f", rotation.x, rotation.y, rotation.z);
	ImGui::Text("Intro: %d", instance->introUniqueID);
	ImGui::Text("Address: %p", instance);

	// Buttons
	if (ImGui::Button("Goto"))
	{
		SkewTo(instance);
	}

	ImGui::SameLine();

	if (ImGui::Button("Bring"))
	{
		auto player = Game::GetPlayerInstance();

#ifndef TR8
		instance->oldPos = player->position;
#endif
		instance->position = player->position;
	}

	ImGui::SameLine();

	if (ImGui::Button("Delete"))
	{
		INSTANCE_ReallyRemoveInstance(instance, 0, false);
	}

#ifdef TR8
	ImGui::SameLine();

	if (ImGui::Button("Unhide"))
	{
		INSTANCE_Post(instance, 7, 0);
	}
#endif

	// Object info
	if (ImGui::CollapsingHeader("Object"))
	{
		auto object = instance->object;

		ImGui::Text("Models: %d", object->numModels);
		ImGui::Text("Animations: %d", object->numAnims);
	}

	// Draw groups
	if (ImGui::CollapsingHeader("Draw groups"))
	{
		static int drawGroup = 0;
		static bool enabled = false;

		ImGui::InputInt("Draw group", &drawGroup);
		ImGui::Checkbox("Enabled", &enabled);

		if (ImGui::Button("Toggle"))
		{
			INSTANCE_HideUnhideDrawGroup(instance, drawGroup, enabled);
		}
	}

	// Animations
	if (ImGui::CollapsingHeader("Animations"))
	{
		static int animation = 0;
		static bool loop = true;

		ImGui::InputInt("Animation", &animation);
		ImGui::Checkbox("Loop", &loop);

		if (ImGui::Button("Play"))
		{
			G2EmulationInstanceSetAnimation(instance, 0, animation, 0, 0);
			G2EmulationInstanceSetMode(instance, 0, loop ? 2 : 1);
		}
	}

	// Messaging
	if (ImGui::CollapsingHeader("Messaging"))
	{
		static int message = 0;
		static int data = 0;

		ImGui::InputInt("Message", &message);
		ImGui::InputInt("Data", &data);

		if (ImGui::Button("Post"))
		{
			INSTANCE_Post(instance, message, data);
		}
	}
}

void InstanceViewer::SkewTo(Instance* instance)
{
	auto player = Game::GetPlayerInstance();

	player->position = instance->position;
}