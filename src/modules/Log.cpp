#include <imgui.h>

#include "Log.h"

void Log::OnMenu()
{
	if (ImGui::BeginMenu("Log"))
	{
		ImGui::MenuItem("Show log", nullptr, &m_show);

		if (ImGui::MenuItem("Clear"))
		{
			Clear();
		}

		ImGui::EndMenu();
	}
}

void Log::OnDraw()
{
	if (m_show)
	{
		ImGui::Begin("Log", &m_show);
		ImGui::BeginChild("Log");

		ImGui::TextUnformatted(m_buffer.begin());

		// Auto scroll to the bottom when already at the bottom
		if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
		{
			ImGui::SetScrollHereY(1.f);
		}

		ImGui::EndChild();
		ImGui::End();
	}
}

void Log::Write(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);

	m_buffer.appendfv(fmt, args);

	va_end(args);
}

void Log::WriteLine(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);

	m_buffer.appendfv(fmt, args);
	m_buffer.append("\n");

	va_end(args);
}

void Log::Clear()
{
	m_buffer.clear();
}