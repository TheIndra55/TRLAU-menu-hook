#pragma once

void __stdcall ScriptPrintInt(int value, int time, bool appendNewLine);

void __stdcall ScriptPrintFloat(float value, int time, bool appendNewLine);

void __cdecl NsCoreBase_PrintString(int instance, int numArgs, void** args);

void __stdcall ScriptLogInt(int value, bool appendNewLine);

void __stdcall ScriptLogFloat(float value, bool appendNewLine);

void __cdecl NsCoreBase_LogString(int instance, int numArgs, void** args);