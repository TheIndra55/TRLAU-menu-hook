#pragma once

#include <string>

#include "Module.h"

#include "instance/Instance.h"

class InstanceModule : public Module
{
private:
	bool m_show = false;
	Instance* m_selected = nullptr;
	char m_filter[64] = "";

	void DrawInstance() const;
	void SkewTo(Instance* instance) const noexcept;
	void UnhideAll() const noexcept;

	static std::string GetBinary(int value);

public:
	void OnMenu();
	void OnDraw();

	Instance* GetSelected() const noexcept { return m_selected; }
};