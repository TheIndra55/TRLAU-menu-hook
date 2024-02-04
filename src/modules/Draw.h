#pragma once
#ifndef TR8

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

	bool m_drawIntro = false;
	bool m_drawFamily = false;
	bool m_drawHealth = false;
	bool m_drawAnimation = false;

	char m_filter[100] = "";

	void DrawInstances();
	void DrawInstance(Instance* instance);
	void DrawEnemyRoute(Instance* instance);
	void DrawMarkUp();
	void DrawCollision(Level* level);
	void DrawCollision(TerrainGroup* group);
	void DrawPortals(Level* level);
	void DrawSignals(Level* level);

public:
	void OnMenu();
	void OnDraw();
	void OnFrame();
};

#endif