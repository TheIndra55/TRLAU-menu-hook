#include <imgui.h>
#include <string>

#include "Materials.h"
#include "render/Material.h"

#include "cdc/sys/HashMap.h"

void Materials::OnMenu()
{
	if (ImGui::BeginMenu("Materials"))
	{
		ImGui::MenuItem("Material editor", nullptr, &m_show);

		ImGui::EndMenu();
	}
}

void Materials::OnDraw()
{
	if (m_show)
	{
		DrawMaterialEditor();
	}
}

void Materials::DrawMaterialEditor()
{
	ImGui::Begin("Materials", &m_show);
	ImGui::Columns(2);

	// Filter
	ImGui::InputText("Material", m_filter, sizeof(m_filter));

	// Material list
	ImGui::BeginChild("MaterialsTree");

	auto materials = (cdc::HashTable<unsigned int, MaterialEntry>*)0x8A6EBC;

	for (int i = 0; i < materials->m_buckets.m_size; i++)
	{
		auto node = materials->m_buckets.m_data[i];

		if (node)
		{ 
			auto id = node->m_data.first;
			auto entry = &node->m_data.second;

			// Check the filter
			if (strlen(m_filter) > 0 && std::to_string(id).find(m_filter) == std::string::npos)
			{
				continue;
			}

			if (ImGui::TreeNodeEx((void*)entry, ImGuiTreeNodeFlags_Leaf, "%d", id))
			{
				if (ImGui::IsItemClicked())
				{
					m_selected = entry->pMaterial;
				}

				ImGui::TreePop();
			}
		}
	}

	ImGui::EndChild();

	// Material editor
	ImGui::NextColumn();

	if (m_selected)
	{
		ImGui::BeginChild("MaterialEditor");

		DrawMaterialEditor(m_selected);

		ImGui::EndChild();
	}

	ImGui::End();
}

void Materials::DrawMaterialEditor(cdc::CommonMaterial* material)
{
	auto data = material->GetMaterialData();

	for (int i = 0; i < data->numParameters; i++)
	{
		ImGui::PushID(i);
		ImGui::InputFloat4("##Parameters", &data->parameters[i * 4]);
		ImGui::PopID();
	}
}