#include <any>
#include <MinHook.h>

#include "ScriptLog.h"

#include "Log.h"
#include "Hook.h"

static void LogValue(const char* fmt, std::any value, bool newLine)
{
	auto log = Hook::GetInstance().GetModule<Log>();
	log->Write(fmt, value);

	if (newLine)
	{
		log->Write("\n");
	}
}

// Script function hooks

void __stdcall ScriptPrintInt(int value, int time, bool appendNewLine)
{
	LogValue("%d", value, appendNewLine);
}

void __stdcall ScriptPrintFloat(float value, int time, bool appendNewLine)
{
	LogValue("%f", value, appendNewLine);
}

void __stdcall ScriptLogInt(int value, bool appendNewLine)
{
	LogValue("%d", value, appendNewLine);
}

void __stdcall ScriptLogFloat(float value, bool appendNewLine)
{
	LogValue("%f", value, appendNewLine);
}

void __cdecl NsCoreBase_PrintString(int instance, int numArgs, void** args)
{
	LogValue("%s", args[0], (__int8)args[1] != 0);
}

void __cdecl NsCoreBase_LogString(int instance, int numArgs, void** args)
{
	LogValue("%s", args[0], (__int8)args[1] != 0);
}

ScriptLog::ScriptLog()
{
	// Insert all hooks
	MH_CreateHook((void*)0x4A3280, ScriptPrintInt, nullptr);
	MH_CreateHook((void*)0x4A32C0, ScriptPrintFloat, nullptr);
	MH_CreateHook((void*)0x4A3200, ScriptLogInt, nullptr);
	MH_CreateHook((void*)0x4A3240, ScriptLogFloat, nullptr);
	MH_CreateHook((void*)0x795E30, NsCoreBase_PrintString, nullptr);
	MH_CreateHook((void*)0x795DB0, NsCoreBase_PrintString, nullptr);

	MH_EnableHook(MH_ALL_HOOKS);
}