#include "ScriptManager.h"

cdc::ScriptManager* cdc::ScriptManager::GetInstance()
{
	return *(ScriptManager**)0xCC4F5C;
}