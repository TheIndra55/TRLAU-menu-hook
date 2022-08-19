#ifndef TR8
#include "d3dterrain.hpp"
#include "../../Hooking.hpp"

TerrainDrawable*(__thiscall* origTerrainDrawable__TerrainDrawable)(TerrainDrawable* _this, XboxPcMaterialStripList* material, TerrainTextureStripInfo* strip, Terrain* terrain, TerrainGroup* terraingroup);

TerrainDrawable* __fastcall TerrainDrawable__TerrainDrawable(TerrainDrawable* _this, int _, XboxPcMaterialStripList* material, TerrainTextureStripInfo* strip, Terrain* terrain, TerrainGroup* terraingroup)
{
	auto ret = origTerrainDrawable__TerrainDrawable(_this, material, strip, terrain, terraingroup);

	if (gWireFrame)
	{
		_this->tpageid |= 0x800;
	}

	return ret;
}
#endif

void InsertTerrainDrawableHooks()
{
#if TRAE
	MH_CreateHook((void*)0x40B9B0, TerrainDrawable__TerrainDrawable, (void**)&origTerrainDrawable__TerrainDrawable);
#elif TR7
	MH_CreateHook((void*)ADDR(0x40B2C0, 0x40ACF0), TerrainDrawable__TerrainDrawable, (void**)&origTerrainDrawable__TerrainDrawable);
#endif
}
