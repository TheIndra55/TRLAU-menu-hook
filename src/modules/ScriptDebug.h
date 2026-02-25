#pragma once

#include "Module.h"
#include "cdc/script/ScriptObject.h"

class ScriptDebug : public Module
{
private:
	bool m_show = false;
	cdc::ScriptObject* m_selected = nullptr;
	char m_filter[12] = "";

	void DrawScriptDebug();
	void DrawScriptDebug(cdc::ScriptObject* object);
	void DrawDataMember(cdc::ScriptObject* object, cdc::DataMember* member);

public:
	void OnMenu();
	void OnDraw();
};