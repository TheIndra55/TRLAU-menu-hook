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
		DrawComboOption("IntroSkip", "Legal screen and intros", { "Don't skip", "Skip legal", "Skip legal and intros" });
		DrawCheckOption("NoCinematicBars", "Disable cinematic bars");
		DrawCheckOption("NoMotionBlur", "Disable motion blur");
	}

	if (ImGui::CollapsingHeader("Camera"))
	{
		DrawSliderOption("CameraSlowSpeed", "Slow speed", 0.f, 1000.f);
		DrawSliderOption("CameraNormalSpeed", "Normal speed", 0.f, 1000.f);
		DrawSliderOption("CameraFastSpeed", "Fast speed", 0.f, 1000.f);
		DrawSliderOption("CameraRollSpeed", "Roll speed", 0.f, 1.f);
	}

	if (ImGui::CollapsingHeader("Controls"))
	{
		DrawSliderOption("SkewSpeed", "Vertical skew speed", 0.f, 1000.f);
	}

	ImGui::End();
}

void Options::DrawCheckOption(const char* name, const char* description) const noexcept
{
	auto option = FindOption(name);
	if (option == nullptr) return;

	if (ImGui::Checkbox(description, (bool*)option->GetValuePtr()))
	{
		option->SaveValue();
	}
}

void Options::DrawSliderOption(const char* name, const char* description, float min, float max) const noexcept
{
	auto option = FindOption(name);
	if (option == nullptr) return;

	if (ImGui::SliderFloat(description, (float*)option->GetValuePtr(), min, max))
	{
		option->SaveValue();
	}
}

void Options::DrawComboOption(const char* name, const char* description, std::vector<const char*> items) const noexcept
{
	auto option = FindOption(name);
	if (option == nullptr) return;

	auto value = (int*)option->GetValuePtr();

	if (ImGui::Combo(description, value, items.data(), items.size()))
	{
		option->SaveValue();
	}
}

void Options::SetVisible(bool visible) noexcept
{
	m_show = visible;
}

void Options::AddOption(BaseOption* option)
{
	m_options.push_back(option);

	// Load the value from the registry
	option->LoadValue();
}

BaseOption* Options::FindOption(const char* name) const noexcept
{
	// TODO use a map for better performance
	for (auto& option : m_options)
	{
		if (option->GetName() == name)
		{
			return option;
		}
	}

	return nullptr;
}