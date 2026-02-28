#include "ScriptObject.h"

#include "util/Hooking.h"

void cdc::ScriptObject::CallFunction(Function* function, int numArgs, void* args, void* retVal)
{
	Hooking::ThisCall(0x57B380, this, function, numArgs, args, retVal);
}