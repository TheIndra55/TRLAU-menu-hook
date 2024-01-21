#pragma once

#include <imgui.h>

#include "Module.h"

class Log : public Module
{
private:
	bool m_show = true;
	ImGuiTextBuffer m_buffer;

public:
	void OnMenu();
	void OnDraw();

	// Logs a message
	void LogMessage(const char* fmt, ...);

	// Clears the log
	void Clear();
};