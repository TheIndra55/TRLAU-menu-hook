#pragma once

#include "Module.h"
#include "level/Stream.h"

class LevelModule : public Module
{
private:
	bool m_eventDebug = false;
	StreamUnit* m_selected = nullptr;

	void DrawEventDebug();
	void DrawEventDebug(StreamUnit* unit);

public:
	LevelModule();

	void OnMenu();
	void OnDraw();
};