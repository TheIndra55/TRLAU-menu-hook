#pragma once

#include <string>

#include "Module.h"

#include "instance/Instance.h"
#include "cdc/resource/IDMap.h"

class InstanceModule : public Module
{
private:
	bool m_show = false;
	Instance* m_selected = nullptr;
	char m_filter[64] = "";
	bool m_hasAnimNames = false;
#ifndef TR8
	cdc::IDMap m_animations;
#endif

	void DrawInstance() const;
	void SkewTo(Instance* instance) const noexcept;
	void UnhideAll() const noexcept;

	const char* GetAnimName(AnimListEntry* anim) const noexcept;

	static std::string GetBinary(int value);

public:
	void OnMenu();
	void OnDraw();
	void OnPostInitialize();

	Instance* GetSelected() const noexcept { return m_selected; }
};