#include <stdio.h>
#include <string>
#include <typeinfo>
#include <imgui.h>

#include "ScriptDebug.h"
#include "util/Helpers.h"

#include "cdc/sys/HashMap.h"
#include "cdc/script/ScriptManager.h"
#include "cdc/script/ScriptType.h"
#include "cdc/script/ScriptSymbol.h"

static const char* s_dataTypes[]
{
	"DTVOID",
	"BOOL",
	"INT",
	"FLOAT",
	"STRING",
	"STATE",
	"ARRAY",
	"DYNARRAY",
	"MAP",
	"UNKNOWN",
	"NATIVE",
	"SCRIPT",
	"STRUCT",
	"STRUCT_REF",
	"ENUM",
	"NTENUM"
};

class Dummy
{
	virtual ~Dummy() = 0;
};

// Gets the RTII type name from the passed pointer
static std::string GetTypeName(uintptr_t* ptr)
{
	if (ptr == nullptr)
	{
		return "";
	}

	try
	{
		return typeid(*(Dummy*)ptr).name();
	}
	catch (std::__non_rtti_object&)
	{
	}

	return "";
}

void ScriptDebug::OnMenu()
{
	if (ImGui::BeginMenu("Script"))
	{
		ImGui::MenuItem("Script objects", nullptr, &m_show);

		ImGui::EndMenu();
	}
}

void ScriptDebug::OnDraw()
{
	if (m_show)
	{
		DrawScriptDebug();
	}
}

void ScriptDebug::DrawScriptDebug()
{
	ImGui::Begin("Script objects", &m_show);
	ImGui::Columns(2);

	// Filter
	ImGui::InputText("Object", m_filter, sizeof(m_filter));

	// Script list
	ImGui::BeginChild("ScriptsTree");

	auto manager = cdc::ScriptManager::GetInstance();
	auto objects = &manager->m_objects;

	for (unsigned int i = 0; i < objects->m_buckets.m_size; i++)
	{
		auto node = objects->m_buckets.m_data[i];

		if (node)
		{
			auto object = node->m_data.first;

			// Check the filter
			if (strlen(m_filter) > 0 && Helpers::ToHex((uintptr_t)object).find(m_filter) == std::string::npos)
			{
				continue;
			}

			auto script = object->m_scriptType;
			auto native = script->m_native;

			if (ImGui::TreeNodeEx((void*)object, ImGuiTreeNodeFlags_Leaf, "%p %s", object, native->m_name))
			{
				if (ImGui::IsItemClicked())
				{
					m_selected = object;
				}

				ImGui::TreePop();
			}
		}
	}

	ImGui::EndChild();

	// Script debug
	ImGui::NextColumn();

	if (m_selected)
	{
		ImGui::BeginChild("ScriptDebug");

		DrawScriptDebug(m_selected);

		ImGui::EndChild();
	}

	ImGui::End();
}

void ScriptDebug::DrawScriptDebug(cdc::ScriptObject* object)
{
	auto script = object->m_scriptType;

	if (script == nullptr)
	{
		m_selected = nullptr;
		return;
	}

	auto data = script->m_streamData;

	ImGui::Text("%p", object);
	ImGui::Text("%d states, %d enums, %d structs, %d members, %d start functions, %d prototypes, %d dependencies",
		data->m_states.size(),
		data->m_enums.size(),
		data->m_structs.size(),
		data->m_members.size(),
		data->m_startFunctionList.size(),
		data->m_prototypes.size(),
		data->m_dependencies.size());

	if (ImGui::CollapsingHeader("Members"))
	{
		for (unsigned int i = 0; i < data->m_members.size(); i++)
		{
			auto member = &data->m_members.m_data[i];
			auto type = member->m_type;
			auto data = (char*)object->m_instance;

			ImGui::Text("%d type: %s", i, s_dataTypes[type.m_type]);
			ImGui::SameLine();

			DrawDataMember(object, member);
		}
	}
}

void ScriptDebug::DrawDataMember(cdc::ScriptObject* object, cdc::DataMember* member)
{
	auto data = &object->m_instance[member->m_offset];

	switch (member->m_type.m_type)
	{
	case cdc::DataType::BOOL:
		ImGui::Text("%s", *data ? "true" : "false");
		break;

	case cdc::DataType::INT:
		ImGui::Text("%d", *(int*)data);
		break;

	case cdc::DataType::FLOAT:
		ImGui::Text("%f", *(float*)data);
		break;

	case cdc::DataType::STRING:
		ImGui::Text("%s", (*(cdc::ScriptSymbol**)&data)->m_string);
		break;

	// TODO ARRAY, DYNARRAY, MAP

	default:
		ImGui::Text("%p %s", *(uintptr_t*)data, GetTypeName(*(uintptr_t**)data).c_str());
		break;
	}
}