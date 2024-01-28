#include <string>
#include <imgui.h>

#include "Instance.h"
#include "instance/Instances.h"
#include "instance/Animation.h"
#include "game/Game.h"

void InstanceModule::OnMenu()
{
	if (ImGui::BeginMenu("Instance"))
	{
		ImGui::MenuItem("Instances", nullptr, &m_show);

		ImGui::EndMenu();
	}
}

void InstanceModule::OnDraw()
{
	if (m_show)
	{
		ImGui::Begin("Instances", &m_show);
		ImGui::Columns(2);

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

void InstanceModule::DrawInstance()
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

		// Code below might crash if the instance got removed
		// so return early
		return;
	}

#ifdef TR8
	ImGui::SameLine();

	if (ImGui::Button("Unhide"))
	{
		INSTANCE_Post(instance, 7, 0);
	}
#endif

	// Transforms
	if (ImGui::CollapsingHeader("Transforms"))
	{
		ImGui::InputFloat3("Position", (float*)&instance->position);
		ImGui::InputFloat3("Rotation", (float*)&instance->rotation);
	}

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
#ifndef TR8
		// Draw the bit representation
		auto binary = GetBinary(~instance->noDrawGroups);

		ImGui::Text("%s", binary.c_str());
#endif

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
#ifndef TR8
		// Show all allocated animation sections with the current playing animation
		for (int section = 0; section < instance->animComponent->mAnimProcessor->mSectionsAllocated; section++)
		{
			// Query the current animation
			auto currentAnim = G2EmulationInstanceQueryAnimation(instance, section);

			// Can be null for some reason
			if (instance->object->animList)
			{
				// (Section): (Anim index) (Anim ID)
				ImGui::Text("Section %d: %d (%X)", section, currentAnim, instance->object->animList[currentAnim].animationID);
			}
			else
			{
				ImGui::Text("Section %d: %d", section, currentAnim);
			}
		}
#endif

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

void InstanceModule::SkewTo(Instance* instance)
{
	auto player = Game::GetPlayerInstance();

	player->position = instance->position;
}

std::string InstanceModule::GetBinary(int value)
{
	std::string binary;

	// Not actually binary, reversed binary
	for (int i = 0; i <= 31; i++)
	{
		binary += (value & (1 << i)) ? "1" : "0";
	}

	return binary;
}