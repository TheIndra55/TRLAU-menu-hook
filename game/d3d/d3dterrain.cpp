#ifndef TR8
#include "d3dterrain.hpp"

#include "../../Hooking.hpp"

void*(__thiscall* GetVertexShader)(void* _this, unsigned int index);

void*(__cdecl* GetDrawListByTpageId)(unsigned int tpageid, bool reflect);

extern void(__cdecl* org_Font__Print)(const char* fmt, ...);

TerrainDrawable*(__thiscall* TerrainDrawableTerrainDrawable)(
	TerrainDrawable* _this, XboxPcMaterialStripList* material, TerrainTextureStripInfo* strip, Terrain* terrain, TerrainGroup* terraingroup);

unsigned int CreateVertexShaderFlags(unsigned int numLights, unsigned int matFlags)
{
	if (numLights)
		numLights = 6;

	auto flags = 4 * (numLights & 7);

	if ((matFlags & 0xC0) == 64)
		flags |= 1;
	else if ((matFlags & 0xC0) == 128)
		flags |= 2;

	if ((matFlags & 1) != 0)
		flags |= 32;

	if ((matFlags & 0x1C) != 0)
	{
		if ((matFlags & 4) != 0)
			flags |= 128;
		if ((matFlags & 8) != 0)
			flags |= 256;
		if ((matFlags & 16) != 0)
			flags |= 512;
	}

	if ((matFlags & 2) != 0)
		flags |= 64;

	return flags;
}

TerrainDrawable* __fastcall TerrainDrawable_TerrainDrawable(
	TerrainDrawable* _this, int _, XboxPcMaterialStripList* material, TerrainTextureStripInfo* strip, Terrain* terrain, TerrainGroup* terraingroup)
{
	_this->terrain = terrain;
	_this->tpageid = 0;
#if TRAE
	_this->scene = 0x66321C;
	_this->__vftable = 0x64FCA4;
#elif TR7
	_this->scene = ADDR(0xF1A88C, 0xF12864);
	_this->__vftable = ADDR(0xF03EAC, 0xEFDE08);
#endif
	_this->sortZ = 0.0;
	_this->material = material;
	_this->textureStrip = strip;
	_this->terrainGroup = terraingroup;
	_this->indexBuffer = nullptr;
	_this->indices = 0;

	_this->tpageid = material->tpageid;

	if ((terraingroup->flags & 0x1100000) != 0)
	{
		_this->tpageid = _this->tpageid & 0xFFF3FFFF | 0x20000;
	}

	if ((_this->tpageid & 0xE0000) == 0x80000 && (_this->tpageid & 0x1E000) == 0x4000)
	{
		FX_Onscreen_Water = 1;
		_this->tpageid = _this->tpageid & 0xFFF0DFFF | 0x2000C000;
	}

	if (gTextureLess)
		_this->tpageid &= 0xFFFFF800;

#if GENERAL_FIXES
	// TODO wireframe for terrain optional
	if (gWireFrame)
		_this->tpageid |= 0x800;
#endif

	if ((terraingroup->flags & 0x80000) != 0)
	{
		_this->tpageid |= 0x100000;
	}

	_this->renderList = GetDrawListByTpageId(_this->tpageid, terraingroup->flags < 0);

	auto lightFlags = _this->textureStrip->lightFlags;
	auto i = 0;

	for (i = 0; lightFlags; i++)
		lightFlags &= lightFlags - 1;

	auto flags = CreateVertexShaderFlags(i, material->flags);
	_this->vertexShader = GetVertexShader(terrainShaders, flags);

	return _this;
}
#endif

void InsertTerrainDrawableHooks()
{
#if TRAE
	GetVertexShader = reinterpret_cast<void*(__thiscall*)(void*, unsigned int)>(0x61C960);
	GetDrawListByTpageId = reinterpret_cast<void*(__cdecl*)(unsigned int, bool)>(0x4158E0);

	MH_CreateHook((void*)0x40B9B0, TerrainDrawable_TerrainDrawable, nullptr);
#elif TR7
	GetVertexShader = reinterpret_cast<void*(__thiscall*)(void*, unsigned int)>(ADDR(0xED1770, 0xECB0B0));
	GetDrawListByTpageId = reinterpret_cast<void*(__cdecl*)(unsigned int, bool)>(ADDR(0x4148D0, 0x414280));

	MH_CreateHook((void*)ADDR(0x40B2C0, 0x40ACF0), TerrainDrawable_TerrainDrawable, (void**)&TerrainDrawableTerrainDrawable);
#endif
}
