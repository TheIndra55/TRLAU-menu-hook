#pragma once

#include "Module.h"

#include "instance/Instance.h"

class InstanceViewer : public Module
{
private:
	bool m_show = false;
	Instance* m_selected = nullptr;

	void DrawInstance();

public:
	void OnMenu();
	void OnDraw();

	Instance* GetSelected() { return m_selected; }
};