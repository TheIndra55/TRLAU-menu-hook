#include <imgui.h>

#include "Debug.h"
#include "Hook.h"
#include "modules/Log.h"
#include "render/Font.h"
#include "level/Event.h"
#include "util/Hooking.h"
#include "game/Game.h"

static Debug* s_debug;

// Event debug print hooks
void EVENT_DisplayString(char* str, int time)
{
	Hook::GetInstance().GetModule<Log>()->WriteLine("%s", str);
}

void EVENT_DisplayStringXY(char* str, int time, int x, int y)
{
	if (s_debug->IsDrawDebug())
	{
		auto font = Font::GetMainFont();

		font->SetCursor(static_cast<float>(x), static_cast<float>(y));
		font->PrintFormatted(str);
	}
}

void EVENT_PrintScalarExpression(int expval, int time)
{
	if (s_debug->IsDrawDebug())
	{
		auto font = Font::GetMainFont();

		font->Print("%d", expval);
	}
}

void EVENT_Trace(char* str, int level)
{
	Hook::GetInstance().GetModule<Log>()->WriteLine("%s", str);
}

Debug::Debug()
{
	s_debug = this;
}

#define OVERRIDE_FUNC(offset, func) *(void**)((char*)globalData + offset) = (void*)func

void Debug::Initialize() const noexcept
{
	// Override all nullsubs in the event functions structure
	auto globalData = (GlobalData*)GET_ADDRESS(0x1076980, 0x7C8A50, 0x000000);

#ifdef TR7
	OVERRIDE_FUNC(304,  EVENT_DisplayString);
	OVERRIDE_FUNC(444,  FONT_Print);
	OVERRIDE_FUNC(504,  EVENT_DisplayString);
	OVERRIDE_FUNC(604,  EVENT_Trace);
	OVERRIDE_FUNC(1232, EVENT_PrintScalarExpression);
	OVERRIDE_FUNC(1336, EVENT_DisplayStringXY);
#else
	OVERRIDE_FUNC(304,  EVENT_DisplayString);
	OVERRIDE_FUNC(464,  FONT_Print);
	OVERRIDE_FUNC(528,  EVENT_DisplayString);
	OVERRIDE_FUNC(628,  EVENT_Trace);
	OVERRIDE_FUNC(1292, EVENT_PrintScalarExpression);
	OVERRIDE_FUNC(1400, EVENT_DisplayStringXY);
#endif
}

void Debug::OnPostInitialize()
{
	Initialize();
}

void Debug::OnMenu()
{
	if (ImGui::BeginMenu("Debug"))
	{
		if (ImGui::MenuItem("Draw debug", nullptr, &m_drawDebug))
		{
			auto gameTracker = Game::GetGameTracker();

			if (m_drawDebug)
			{
				gameTracker->debugFlags2 |= 0x100;
			}
			else
			{
				gameTracker->debugFlags2 &= ~0x100;
			}
		}

		ImGui::MenuItem("Debug keypad", nullptr, (bool*)GET_ADDRESS(0x107696C, 0x7C8A3C, 0x000000));

		ImGui::EndMenu();
	}
}