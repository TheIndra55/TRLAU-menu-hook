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

	// Instance options
	bool m_drawIntro = false;
	bool m_drawFamily = false;
	bool m_drawHealth = false;
	bool m_drawAnimation = false;

	char m_filter[100] = "";

	// Collision options
	bool m_drawPlayerCollision = true;
	bool m_drawEnemyCollision = true;

	int m_terrainGroup = -1;

	void DrawInstances();
	void DrawInstance(Instance* instance);
	void DrawEnemyRoute(Instance* instance);
	void DrawMarkUp();
	void DrawCollision(Level* level);
	void DrawCollision(TerrainGroup* group);
	void DrawPortals(Level* level);
	void DrawSignals(Level* level);

	static std::string FlagsToString(unsigned int flags);

public:
	void OnMenu();
	void OnDraw();
	void OnFrame();
};