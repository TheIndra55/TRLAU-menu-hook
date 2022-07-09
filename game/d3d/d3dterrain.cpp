#ifndef TR8
#include "d3dterrain.hpp"

#include "../../Hooking.hpp"

unsigned int(__fastcall* CreateVertexShaderFlags)(unsigned int numLights, char matFlags);

int(__thiscall* GetVertexShader)(int _this, unsigned int index);

int(__cdecl* GetDrawListByTpageId)(unsigned int tpageid, bool reflect);

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

	auto tpageid = material->tpageid;
	_this->tpageid = tpageid;

	if ((terraingroup->flags & 0x1100000) != 0)
	{
		_this->tpageid = tpageid & 0xFFF3FFFF | 0x20000;
	}

	if ((tpageid & 0xE0000) == 0x80000 && (tpageid & 0x1E000) == 0x4000)
	{
		FX_Onscreen_Water = 1;
		_this->tpageid = tpageid & 0xFFF0DFFF | 0x2000C000;
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
	for (int i = 0; lightFlags; i++)
		lightFlags &= lightFlags - 1;

	auto flags = CreateVertexShaderFlags(0, material->flags);
	_this->vertexShader = GetVertexShader(terrainShaders, flags);

	return _this;
}
#endif

void InsertTerrainDrawableHooks()
{
#if TRAE
	CreateVertexShaderFlags = reinterpret_cast<unsigned int(__fastcall*)(unsigned int, char)>(0x40B5A0);
	GetVertexShader = reinterpret_cast<int(__thiscall*)(int, unsigned int)>(0x61C960);
	GetDrawListByTpageId = reinterpret_cast<int(__cdecl*)(unsigned int, bool)>(0x4158E0);

	MH_CreateHook((void*)0x40B9B0, TerrainDrawable_TerrainDrawable, nullptr);
#elif TR7
	CreateVertexShaderFlags = reinterpret_cast<unsigned int(__fastcall*)(unsigned int, char)>(ADDR(0x40AE90, 0x40A8E0));
	GetVertexShader = reinterpret_cast<int(__thiscall*)(int, unsigned int)>(ADDR(0xED1770, 0xECB0B0));
	GetDrawListByTpageId = reinterpret_cast<int(__cdecl*)(unsigned int, bool)>(ADDR(0x4148D0, 0x414280));

	MH_CreateHook((void*)ADDR(0x40B2C0, 0x40ACF0), TerrainDrawable_TerrainDrawable, nullptr);
#endif
}
