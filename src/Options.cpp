#include <imgui.h>

#include "Options.h"
#include "Hook.h"

BaseOption::BaseOption(const char* name) : m_name(name)
{
}

Options::Options() : m_options()
{
}

void Options::OnDraw()
{
	if (m_show)
	{
		DrawOptions();
	}
}

void Options::DrawOptions() noexcept
{
	ImGui::Begin("Options", &m_show);

	if (ImGui::CollapsingHeader("General"))
	{
		DrawOption("DisableIntro", "Disable legal screen and intros");
		DrawOption("NoCinematicBars", "Disable cinematic bars");
		DrawOption("NoMotionBlur", "Disable motion blur");
	}

	if (ImGui::CollapsingHeader("Camera"))
	{
		DrawOption("CameraSlowSpeed", "Slow speed", 0.f, 1000.f);
		DrawOption("CameraNormalSpeed", "Normal speed", 0.f, 1000.f);
		DrawOption("CameraFastSpeed", "Fast speed", 0.f, 1000.f);
		DrawOption("CameraRollSpeed", "Roll speed", 0.f, 1.f);
	}

	if (ImGui::CollapsingHeader("Controls"))
	{
		DrawOption("SkewSpeed", "Vertical skew speed", 0.f, 1000.f);
	}

	ImGui::End();
}

void Options::DrawOption(const char* name, const char* description, float min, float max) const noexcept
{
	// TODO use a map for better performance
	for (auto& option : m_options)
	{
		if (option->GetName() == name)
		{
			DrawOption(option, description, min, max);
		}
	}
}

void Options::DrawOption(BaseOption* option, const char* description, float min, float max) const noexcept
{
	auto type = option->GetType();

	if (type == typeid(bool).hash_code())
	{
		if (ImGui::Checkbox(description, (bool*)option->GetValuePtr()))
		{
			option->SaveValue();
		}
	}

	if (type == typeid(float).hash_code())
	{
		if (ImGui::SliderFloat(description, (float*)option->GetValuePtr(), min, max))
		{
			option->SaveValue();
		}
	}
}

void Options::Show() noexcept
{
	m_show = true;
}

void Options::AddOption(BaseOption* option)
{
	m_options.push_back(option);

	// Load the value from the registry
	option->LoadValue();
}