#include <imgui.h>
#include <MinHook.h>

#include "Render.h"
#include "util/Hooking.h"

#include "cdc/render/PCDeviceManager.h"
#include "cdc/render/PCStateManager.h"

#ifndef TR8
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
#else
static bool s_wireframe = false;

static void(__thiscall* s_PCScene_Draw)(int, int, int);

static void __fastcall PCScene_Draw(int a1, void*, int a2, int a3)
{
	auto device = cdc::PCDeviceManager::GetInstance();
	auto state = *(cdc::PCStateManager**)((char*)device + 644); // TODO

	if (s_wireframe)
	{
		state->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	}

	s_PCScene_Draw(a1, a2, a3);

	// Reset
	if (s_wireframe)
	{
		state->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}
}
#endif

Render::Render()
{
#ifndef TR8
	// Insert terrain wireframe hook
	MH_CreateHook((void*)GET_ADDRESS(0x40ACF0, 0x40B9B0, 0x000000), TerrainDrawable_TerrainDrawable, (void**)&s_TerrainDrawable_TerrainDrawable);
#else
	MH_CreateHook((void*)0x5150E0, PCScene_Draw, (void**)&s_PCScene_Draw);
#endif
	MH_EnableHook(MH_ALL_HOOKS);
}

void Render::OnMenu()
{
	if (ImGui::BeginMenu("Render"))
	{
#ifndef TR8
		ImGui::MenuItem("Wireframe", nullptr, (bool*)GET_ADDRESS(0x1075BD4, 0x7C7CD4, 0x000000));
		ImGui::MenuItem("Terrain wireframe", nullptr, &s_terrainWireframe);
#else
		ImGui::MenuItem("Wireframe", nullptr, &s_wireframe);
#endif

		ImGui::EndMenu();
	}
}