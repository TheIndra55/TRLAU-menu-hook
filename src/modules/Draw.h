#pragma once

#include "Module.h"

struct Level;
struct Instance;
struct TerrainGroup;

class Draw : public Module
{
private:
	bool m_drawInstances = false;
	bool m_drawMarkUp = false;
	bool m_drawEnemyRouting = false;
	bool m_drawCollision = false;
	bool m_drawPortals = false;
	bool m_drawSignals = false;
	bool m_drawTriggers = false;

	// Instance options
	bool m_drawIntro = false;
	bool m_drawFamily = false;
	bool m_drawHealth = false;
	bool m_drawAnimation = false;

	char m_filter[100] = "";

	// Collision options
	bool m_drawPlayerCollision = true;
	bool m_drawEnemyCollision = true;
	bool m_cull = true;

	int m_terrainGroup = -1;

	void DrawInstances();
	void DrawInstance(Instance* instance);
	void DrawEnemyRoute(Instance* instance);
	void DrawMarkUp();
	void DrawCollision(Level* level);
	void DrawCollision(TerrainGroup* group, int flags);
	void DrawPortals(Level* level);
	void DrawSignals(Level* level);
	void DrawTriggers();

	static std::string FlagsToString(unsigned int flags);

public:
	void OnMenu();
	void OnDraw();
	void OnFrame();
};

#ifndef TR8
constexpr auto kEnemyCollision = 0x4000;
#else
constexpr auto kEnemyCollision = 0x800;
#endif

constexpr auto kCollisionFar = 20000;