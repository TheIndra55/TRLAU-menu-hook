#pragma once

#include "Module.h"
#include "render/Material.h"

class Materials : public Module
{
private:
	bool m_show = false;
	cdc::CommonMaterial* m_selected = nullptr;
	char m_filter[12] = "";

	void DrawMaterialEditor();
	void DrawMaterialEditor(cdc::CommonMaterial* material);

public:
	void OnDraw();
	void OnMenu();
};