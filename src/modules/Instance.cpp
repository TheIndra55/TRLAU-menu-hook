#include <string>
#include <imgui.h>

#include "Instance.h"
#include "instance/Instances.h"
#include "instance/Animation.h"
#include "game/Game.h"
#include "file/FileSystem.h"
#include "cdc/resource/IDMap.h"

void InstanceModule::OnMenu()
{
	if (ImGui::BeginMenu("Instance"))
	{
		ImGui::MenuItem("Instances", nullptr, &m_show);

#ifdef TR8
		if (ImGui::MenuItem("Unhide all"))
		{
			UnhideAll();
		}
#endif

		ImGui::EndMenu();
	}
}

void InstanceModule::OnPostInitialize()
{
#ifdef TR8
	// TR8 has always anim names
	m_hasAnimNames = true;
#else
	// Check if we have an animlist.ids
	auto fileSystem = GetFS();

	if (fileSystem->FileExists("pc-w\\animlist.ids"))
	{
		// Read the IDMap
		m_animations.Open("pc-w\\animlist.ids", fileSystem);

		m_hasAnimNames = true;
	}
#endif
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

void InstanceModule::DrawInstance() const
{
	auto instance = m_selected;
	auto data = (ObjectData*)instance->data;

	ImGui::Text("%s", instance->object->name);

	auto position = instance->position;
	auto rotation = instance->rotation;

	ImGui::Text("Position: %f %f %f", position.x, position.y, position.z);
	ImGui::Text("Rotation: %f %f %f", rotation.x, rotation.y, rotation.z);
	ImGui::Text("Intro: %d", instance->introUniqueID);
	ImGui::Text("Address: %p", instance);

	if (data)
	{
		ImGui::Text("Family: %d", data->family);
	}

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

		ImGui::Text("ID: %d", object->uniqueID);
		ImGui::Text("Models: %d", object->numModels);
		ImGui::Text("Animations: %d", object->numAnims);
	}

	// Draw groups
	if (ImGui::CollapsingHeader("Draw groups"))
	{
		// Draw the bit representation
		auto binary = GetBinary(~instance->noDrawGroups);

		ImGui::Text("%s", binary.c_str());

		static int drawGroup = 0;
		ImGui::InputInt("Draw group", &drawGroup);

		if (ImGui::Button("Toggle"))
		{
			INSTANCE_HideUnhideDrawGroup(instance, drawGroup, (instance->noDrawGroups & (1 << drawGroup)));
		}
	}

	// Animations
	if (ImGui::CollapsingHeader("Animations"))
	{
		// Show all allocated animation sections with the current playing animation
		for (int section = 0; section < instance->animComponent->mAnimProcessor->mSectionsAllocated; section++)
		{
			// Query the current animation
			auto currentAnim = G2EmulationInstanceQueryAnimation(instance, section);

			// Can be null for some reason
			if (instance->object->animList && currentAnim != 0xffff)
			{
				auto animEntry = &instance->object->animList[currentAnim];

				// (Section): (Anim index) (Anim name) (Anim ID)
				if (!m_hasAnimNames)
				{
					ImGui::Text("Section %d: %d (%X)", section, currentAnim, animEntry->animationID);
				}
				else
				{
					auto name = GetAnimName(animEntry);
					ImGui::Text("Section %d: %d %s (%X)", section, currentAnim, name, animEntry->animationID);
				}
			}
			else
			{
				ImGui::Text("Section %d: %d", section, currentAnim);
			}
		}

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

void InstanceModule::SkewTo(Instance* instance) const noexcept
{
	auto player = Game::GetPlayerInstance();

	player->position = instance->position;
}

void InstanceModule::UnhideAll() const noexcept
{
	Instances::Iterate([](Instance* instance)
		{
			INSTANCE_Post(instance, 7, 0);
		});
}

const char* InstanceModule::GetAnimName(AnimListEntry* anim) const noexcept
{
#ifdef TR8
	return anim->debugName;
#else
	return m_animations.GetName(anim->animationID);
#endif
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