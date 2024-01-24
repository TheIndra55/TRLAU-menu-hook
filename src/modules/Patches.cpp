#include <Hooking.Patterns.h>

#include "Patches.h"
#include "util/Hooking.h"

Patches::Patches()
{
#ifndef TR8
	RemoveIntro();
#endif
}

void Patches::RemoveIntro()
{
	auto match = hook::pattern("8D 0C 8D 03 00 00 00 89 0D").count(1);
	auto mainState = *match.get_first<int>(9);

	// Nop out the code setting the main state to intros
	Hooking::Nop(match.get_first(), 13);

	// mov [mainState], 6
	Hooking::Patch(match.get_first(), { 0xC7, 0x05, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00 });
	Hooking::Patch(match.get_first(2), mainState);
}

void Patches::OnInput(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// Remove the quit message
#ifdef TRAE
	if (msg == WM_CLOSE)
	{
		PostQuitMessage(0);
	}
#endif
}