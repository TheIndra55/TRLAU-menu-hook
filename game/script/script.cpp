#include "../../Hooking.hpp"
#include "script.hpp"

void __stdcall ScriptPrintInt(int value, int time, bool appendNewLine)
{
	auto menu = Hooking::GetInstance().GetMenu();

	menu->Log("%d", value);

	if (appendNewLine)
	{
		menu->Log("\n");
	}
}

void __stdcall ScriptPrintFloat(float value, int time, bool appendNewLine)
{
	// these should actually print on the screen but didn't look into Font:: code yet so now they log to console
	auto menu = Hooking::GetInstance().GetMenu();

	menu->Log("%f", value);

	if (appendNewLine)
	{
		menu->Log("\n");
	}
}

void __cdecl NsCoreBase_PrintString(int instance, int numArgs, void** args)
{
	auto menu = Hooking::GetInstance().GetMenu();

	menu->Log("%s", (char*)args[0]);

	// time is args[1]

	if ((__int8)args[1] != 0)
	{
		menu->Log("\n");
	}
}

void __stdcall ScriptLogInt(int value, bool appendNewLine)
{
	auto menu = Hooking::GetInstance().GetMenu();

	menu->Log("%d", value);

	if (appendNewLine)
	{
		menu->Log("\n");
	}
}

void __stdcall ScriptLogFloat(float value, bool appendNewLine)
{
	auto menu = Hooking::GetInstance().GetMenu();

	menu->Log("%f", value);

	if (appendNewLine)
	{
		menu->Log("\n");
	}
}

void __cdecl NsCoreBase_LogString(int instance, int numArgs, void** args)
{
	auto menu = Hooking::GetInstance().GetMenu();

	menu->Log("%s", (char*)args[0]);

	if ((__int8)args[1] != 0)
	{
		menu->Log("\n");
	}
}