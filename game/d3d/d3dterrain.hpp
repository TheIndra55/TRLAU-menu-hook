#include "../../Game.hpp"
#include "../../util.hpp"

struct TerrainTextureStripInfo
{
	int vertexCount;
	int vmoObjectIndex;
	float texScrollOffset;
	unsigned int lightFlags;
};

struct XboxPcMaterialStripList
{
	unsigned int tpageid;
	unsigned int flags;
	unsigned int vbBaseOffset;
	unsigned int numStrips;
};

struct TerrainDrawable
{
	// cdc::PCDrawable shit
	int __vftable;
	int scene;
	float sortZ;

	// TerrainDrawable
	XboxPcMaterialStripList* material;
	TerrainTextureStripInfo* textureStrip;
	Terrain* terrain;
	TerrainGroup* terrainGroup;
	unsigned int tpageid;
	void* indexBuffer;
	void* vertexShader;
	void* renderList;
	unsigned int indices;
};

void InsertTerrainDrawableHooks();

#if TRAE

#define gTextureLess VAR_U_(0x7C7CD0, int)
#define gWireFrame VAR_U_(0x7C7CD4, int)
#define terrainShaders VAR_U_(0x69AB00, void*)

#define FX_Onscreen_Water VAR_U_(0x812B5C, int)

#elif TR7

#define gTextureLess VAR_U_(ADDR(0x107E57C, 0x1075BD0), int)
#define gWireFrame VAR_U_(ADDR(0x107E580, 0x1075BD4), int)
#define terrainShaders VAR_U_(ADDR(0xF51400, 0xF48A60), void*)

#define FX_Onscreen_Water VAR_U_(ADDR(0x10C9D8C, 0x10C09DC), int)

#endif
