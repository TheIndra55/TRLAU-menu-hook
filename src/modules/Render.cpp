#include <imgui.h>
#include <MinHook.h>

#include "Render.h"
#include "util/Hooking.h"

static bool s_terrainWireframe = false;

// Code for terrain wireframe
class TerrainDrawable
{
public:
	char pad1[28];
	unsigned int m_tpageid;
};

static TerrainDrawable* (__thiscall* s_TerrainDrawable_TerrainDrawable)(TerrainDrawable*, void*, void*, void*, void*);

static TerrainDrawable* __fastcall TerrainDrawable_TerrainDrawable(TerrainDrawable* drawable, void*, void* material, void* textureStrip, void* terrain, void* terrainGroup)
{
	auto ret = s_TerrainDrawable_TerrainDrawable(drawable, material, textureStrip, terrain, terrainGroup);

	if (s_terrainWireframe)
	{
		drawable->m_tpageid |= 0x800;
	}

	return ret;
}

Render::Render()
{
	// Insert terrain wireframe hook
	MH_CreateHook((void*)GET_ADDRESS(0x40ACF0, 0x40B9B0, 0x000000), TerrainDrawable_TerrainDrawable, (void**)&s_TerrainDrawable_TerrainDrawable);
	MH_EnableHook(MH_ALL_HOOKS);
}

void Render::OnMenu()
{
	if (ImGui::BeginMenu("Render"))
	{
		// TODO Wireframe for Underworld
		ImGui::MenuItem("Wireframe", nullptr, (bool*)GET_ADDRESS(0x1075BD4, 0x7C7CD4, 0x000000));
		ImGui::MenuItem("Terrain wireframe", nullptr, &s_terrainWireframe);

		ImGui::EndMenu();
	}
}