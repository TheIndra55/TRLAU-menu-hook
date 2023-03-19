#pragma once

#include <Windows.h>
#include <functional>

// hack until everything is patterns
#if TR7 && RETAIL_VERSION
	#define ADDR(debug, retail) retail
#elif TR7
	#define ADDR(debug, retail) debug
#endif

#if TRAE
	#define GAMETRACKER 0x838330
	#define GAMETRACKER_BASE_AREA (GAMETRACKER + 204)
	#define MAX_UNIT_LEN 16
	#define PLAYERINSTANCE 0x83833C

	#define DISKFS 0x838890
	#define ARCHIVEFS 0x83888C
	#define CONFIGNAME "TRAE"
	#define INSTANCELIST 0x817D64
	#define GLOBALDATA 0x007C8A50
	#define CAMERAMODE 0x850984
#elif TR8
	#define GAMETRACKER 0x00E7F088
	#define GAMETRACKER_BASE_AREA (GAMETRACKER + 56)
	#define MAX_UNIT_LEN 128
	#define PLAYERINSTANCE 0xE7F094

	#define DISKFS 0x9CE27C
	#define ARCHIVEFS 0x9CE278
	#define CONFIGNAME "TR8"
	#define INSTANCELIST 0xD98D54
	#define GLOBALDATA 0
#elif TR7
	#define GAMETRACKER ADDR(0x10EE730, 0x10E5370)
	#define GAMETRACKER_BASE_AREA (GAMETRACKER + 204)

	#define MAX_UNIT_LEN 16
	#define PLAYERINSTANCE (GAMETRACKER + 12)

	#define DISKFS ADDR(0x10EEC80, 0x10E58C0)
	#define ARCHIVEFS ADDR(0x10EEC7C, 0x10E58BC)
	#define CONFIGNAME "TR7"
	#define INSTANCELIST ADDR(0x10CEE64, 0x10C5AA4)
	#define GLOBALDATA ADDR(0x107F920, 0x1076980)
	#define CAMERAMODE ADDR(0, 0x10FC974)
#else
	#error "No game specified, set TRAE for Anniversary, TR7 for Legend or TR8 for Underworld"
#endif

namespace cdc
{
	struct Vector
	{
		float x, y, z, w;
	};
}

struct ObjectTracker
{
	__int32 resolveObj;
	char* objectName;
	DWORD* object;
	__int16 id;
	__int16 status;
};

#if TRAE || TR7
struct Model;

struct AnimListEntry
{
	__int16 animationID;
	__int16 animationNumber;
};

struct Object
{
	char pad1[24];
	__int16 numModels;
	__int16 numAnims;
	__int16 numAnimPatterns;
	__int16 pad2;
	Model** modelList;
	AnimListEntry* animList;
	char pad3[32];
	char* name;
};

struct Instance
{
	__int64 node;
	Instance* next;
	Instance* prev;
	cdc::Vector position;
	cdc::Vector oldPosition;
	cdc::Vector rotation;
	cdc::Vector oldRotation;
	cdc::Vector scale;
	cdc::Vector shadowPosition;
	cdc::Vector centerOfMass;
	__int32 matrix;
	__int32 oldMatrix;
	__int16 unk7;
	__int8 unk8;
	__int8 unk9;
	__int16 introNum;
	int unk10;
	Object* object;
	char padding[312];
	int introUniqueID;
};
#elif TR8
struct Object
{
	char padding[96];
	char* name;
};

struct Instance
{
	__int64 node;
	Instance* next;
	__int32 prev;
	Object* object;
	char padding[12];
	cdc::Vector position;
	cdc::Vector rotation;
	char padding2[24];
	int introUniqueID;
};
#endif

struct Level;
struct StreamUnit;
struct Signal;

struct MeshVertex;
struct MeshVertex32;

struct StreamUnitPortal
{
	char tolevelname[30];
	__int16 toSignalID;
	__int16 MSignalID;
	__int16 streamID;
	unsigned __int16* closeVertList;
	float activeDistance;
	StreamUnit* toStreamUnit;
	cdc::Vector min;
	cdc::Vector max;
	cdc::Vector portalquad[4];
	cdc::Vector normal;
};

struct Mesh
{
	char m_box[0x20];
	cdc::Vector m_position;
	void* m_vertices;
	void* m_faces;
	int m_root;
	void* m_clientData;
	unsigned __int16 m_vertexType;
	unsigned __int16 m_numNodes;
	unsigned __int16 m_numFaces;
	unsigned __int16 m_numVertices;
};

struct TerrainGroup
{
	cdc::Vector globalOffset;
	cdc::Vector localOffset;
	int flags;
	int ID;
	int uniqueID;
	int splineID;
	Instance* instanceSpline;
	Level* level;
	Mesh* mesh;
	char pad[0x74];
};

struct Terrain
{
	__int16 UnitChangeFlags;
	__int16 spad;
	int numIntros;
	int introList;
	int numStreamUnitPortals;

	// all portals
	StreamUnitPortal* streamUnitPortals;

	int numTerrainGroups;
	TerrainGroup* terrainGroups;
	TerrainGroup* signalTerrainGroup;
	Signal* signals;
};

struct Level
{
	Terrain* terrain;
	char pad[140];
	Signal* SignalListStart;
	__int16* SignalIDList;
	void* splineCameraData;
	int reloc; // void* to script PE
};

struct StreamUnit
{
	int id;
	__int8 used;
	__int8 unitHidden;
	__int16 flags;
	Level* level;
};

struct Model
{
	int version;
	int numSegments;
	int numVirtSegments;
	int segmentList;
	cdc::Vector modelScale;
	int numVertices;
	int vertexList;
	int numNormals;
	int normalList;
	int numFaces;
	int faceList;
};

struct MeshVertex
{
	__int16 x;
	__int16 y;
	__int16 z;
};

struct MeshVertex32
{
	float x;
	float y;
	float z;
	float w;
};

struct BBox
{
	cdc::Vector bMin;
	cdc::Vector bMax;
};

struct SignalFace
{
	unsigned __int16 i0;
	unsigned __int16 i1;
	unsigned __int16 i2;
	char adjacencyFlags;
	char collisionFlags;
	unsigned __int16 id;
};

struct IndexedFace
{
	unsigned __int16 i0;
	unsigned __int16 i1;
	unsigned __int16 i2;
	char adjacencyFlags;
	char collisionFlags;
	char clientFlags;
	char materialType;
};

struct DRAWVERTEX
{
	float x;
	float y;
	float z;
	int color;
	float u;
	float v;
	__int64 anPad;
};

#define RGBA(r, g, b, a) r | (g << 8) | (b << 16) | (a << 24)

// taken from TRAE.pdb
// should be compatible with TR7
enum MUD_FLAGS
{
	MUD_FLAG_NONE = 0,
	MUD_FLAG_POLYLINE = 1,
	MUD_FLAG_INSTANCE = 2,
	MUD_FLAG_WRAPS = 4,
	MUD_FLAG_PATTERN_INTERACT = 0x2000,
	MUD_FLAG_ENEMY_JUMP_ACROSS = 0x4000,
	MUD_FLAG_ENEMY_JUMP_DOWN = 0x8000,
	MUD_FLAG_ENEMY_JUMP_UP = 0x10000,
	MUD_FLAG_ATTACHPOINTS = 0x20000,
	MUD_FLAG_PERCH = 0x40000,
	MUD_FLAG_ROPE = 0x80000,
	MUD_FLAG_WATER = 0x100000,
	MUD_FLAG_CLOSEJUMPTARGET = 0x200000,
	MUD_FLAG_LADDER = 0x400000,
	MUD_FLAG_MANTLEDOWN = 0x800000,
	MUD_FLAG_ZIPLINE = 0x1000000,
	MUD_FLAG_HORIZPOLE = 0x2000000,
	MUD_FLAG_VERTPOLE = 0x4000000,
	MUD_FLAG_WALLVERTPOLE = 0x8000000,
	MUD_FLAG_ENEMYDROPDOWN = 0x10000000,
	MUD_FLAG_JUMPLANDING = 0x20000000,
	MUD_FLAG_LEDGEGRAB = 0x40000000,
	MUD_FLAG_DISABLED = 0x80000000,
	MUD_FLAG_ALL_TYPES = 0x0FFFDC000,
	MUD_FLAG_ALL = 0x0FFFFFFFF
};

struct MudFlag
{
	int flag;
	const char* name;
};

constexpr MudFlag mudFlags[22]
{
	MudFlag { MUD_FLAG_NONE, "NONE | " },
	MudFlag { MUD_FLAG_INSTANCE, "INSTANCE | " },
	MudFlag { MUD_FLAG_WRAPS, "WRAPS | " },
	MudFlag { MUD_FLAG_PATTERN_INTERACT, "PATTERN_INTERACT | " },
	MudFlag { MUD_FLAG_ENEMY_JUMP_ACROSS, "ENEMY_JUMP_ACROSS | " },
	MudFlag { MUD_FLAG_ENEMY_JUMP_DOWN, "ENEMY_JUMP_DOWN | " },
	MudFlag { MUD_FLAG_ENEMY_JUMP_UP, "ENEMY_JUMP_UP | " },
	MudFlag { MUD_FLAG_ATTACHPOINTS, "ATTACHPOINTS | " },
	MudFlag { MUD_FLAG_PERCH, "PERCH | " },
	MudFlag { MUD_FLAG_ROPE, "ROPE | " },
	MudFlag { MUD_FLAG_WATER, "WATER | " },
	MudFlag { MUD_FLAG_CLOSEJUMPTARGET, "CLOSEJUMPTARGET | " },
	MudFlag { MUD_FLAG_LADDER, "LADDER | " },
	MudFlag { MUD_FLAG_MANTLEDOWN, "MANTLEDOWN | " },
	MudFlag { MUD_FLAG_ZIPLINE, "ZIPLINE | " },
	MudFlag { MUD_FLAG_HORIZPOLE, "HORIZPOLE | " },
	MudFlag { MUD_FLAG_VERTPOLE, "VERTPOLE | " },
	MudFlag { MUD_FLAG_WALLVERTPOLE, "WALLVERTPOLE | " },
	MudFlag { MUD_FLAG_ENEMYDROPDOWN, "ENEMYDROPDOWN | " },
	MudFlag { MUD_FLAG_JUMPLANDING, "JUMPLANDING | " },
	MudFlag { MUD_FLAG_LEDGEGRAB, "LEDGEGRAB | " },
	MudFlag { MUD_FLAG_DISABLED, "DISABLED | " },
};

class Game
{
public:
	static void Initialize();

	static void ResetGame(int a1);

	static void PushScreen(int screenId, int a2);
	static int GetTopScreenID();
	static void PushOkDialog(const char* text, void(__cdecl* fn)(), int a3, int a4);
	static void PopScreen();

	static void InstancePost(Instance* instance, int a2, int data);
	static int InstanceQuery(Instance* instance, int a2);
	static Instance* InstanceFind(int intro);

	static bool CheckChapter(char* chapter);
	static void SwitchPlayerCharacter();
	static DWORD AnimDataSomething(int a1, int a2, int a3);
	static void InstanceSetEventAnimPlaying(Instance* instance, int a2);

	static void IncreaseHealth(float amount);
	static void TriggerUiFadeGroup(int group);

	static void SetGameValue(int key, float val, bool apply);
	static double GetGameValue(int key);

	static bool m_binoculars;
	static void ToggleBinoculars();

	static void PlayerTurnGold();
	static Instance* BirthObjectNoParent(int unitId, cdc::Vector* position, cdc::Vector* rotation, DWORD* introData, DWORD* object, int modelnum, int initEffects);

	static int GetObjectID(char* name);
};

class Stream
{
public:
	static ObjectTracker* GetObjectTrackerByName(char* name);
	static bool PollLoadQueue();
};

char* localstr_get(int a1);

void memcpyProtectedSection(void* dst, const void* src, size_t size);
