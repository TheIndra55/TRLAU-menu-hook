#include <imgui.h>

#include "InstanceViewer.h"
#include "instance/Instances.h"

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

		// Instance list
		ImGui::BeginChild("InstancesTree");

		Instances::Iterate([this](Instance* instance)
			{
				if (ImGui::TreeNodeEx((void*)instance, ImGuiTreeNodeFlags_Leaf, "%d %s", instance->introUniqueID, instance->object->name))
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

	ImGui::Text("Position: %f %f %f", position.x, position.y, position.z);
	ImGui::Text("Rotation: %f %f %f", rotation.x, rotation.y, rotation.z);
	ImGui::Text("Intro: %d", instance->introUniqueID);
	ImGui::Text("Address: %p", instance);
}