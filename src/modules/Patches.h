#pragma once

#include <filesystem>

#include "Module.h"
#include "Options.h"
#include "util/Helpers.h"

template <typename T>
class Patch
{
private:
	T m_value;

	static int GetPatchValue(const char* name, int defaultValue) noexcept
	{
		auto path = std::filesystem::current_path() / "patches.ini";

		// Read the value from the ini file
		char value[20];
		GetPrivateProfileStringA("Patches", name, NULL, value, sizeof(value), path.string().c_str());

		return Helpers::StringToInt(std::string(value), defaultValue);
	}

public:
	Patch(const char* name, T defaultValue)
	{
		m_value = GetPatchValue(name, defaultValue);
	}

	T GetValue() const noexcept
	{
		return m_value;
	}
};

class Patches : public Module
{
private:
	Option<bool> m_disableIntro{ "DisableIntro", true };
	Option<bool> m_noCinematicBars{ "NoCinematicBars", true };
	Option<bool> m_noMotionBlur{ "NoMotionBlur", false };

	Patch<unsigned int> m_heapSize{ "HeapSize", 0 };
#ifdef TR7
	Patch<unsigned int> m_shadowMapSize{ "MaxShadowMap", 0 };
#endif

	void RemoveIntro() const noexcept;
	void PatchHeapSize() const noexcept;
	void PatchShadowMap() const noexcept;

public:
	Patches();

	bool IsNoMotionBlur() const noexcept { return m_noMotionBlur.GetValue(); }
	bool IsNoCinematicBars() const noexcept { return m_noCinematicBars.GetValue(); }

	void OnInput(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};