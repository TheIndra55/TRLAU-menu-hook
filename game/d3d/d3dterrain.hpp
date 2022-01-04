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
	int vertexShader;
	int renderList;
	unsigned int indices;
};

void InsertTerrainDrawableHooks();

#if TRAE

#define gTextureLess VAR_U_(0x7C7CD0, int)
#define gWireFrame VAR_U_(0x7C7CD4, int)
#define terrainShaders VAR_U_(0x69AB00, int)

#define FX_Onscreen_Water VAR_U_(0x812B5C, int)

#elif TR7

#define gTextureLess VAR_U_(0x107E57C, int)
#define gWireFrame VAR_U_(0x107E580, int)
#define terrainShaders VAR_U_(0xF51400, int)

#define FX_Onscreen_Water VAR_U_(0x10C9D8C, int)

#endif
