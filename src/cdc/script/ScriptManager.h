#pragma once

#include "cdc/sys/RCObject.h"
#include "cdc/sys/HashSet.h"
#include "ScriptObject.h"

namespace cdc
{
	class ScriptManager : RCObject
	{
	public:
		int field_8;
		int field_C;
		int field_10;
		int field_14;
		int field_18;
		int field_1C;
		int field_20;
		int field_24;
		int field_28;
		int field_2C;
		int field_30;
		HashSet<ScriptObject*> m_threads;
		int field_44;
		HashSet<ScriptObject*> m_objects;
		int field_5C;

		static ScriptManager* GetInstance();
	};
}