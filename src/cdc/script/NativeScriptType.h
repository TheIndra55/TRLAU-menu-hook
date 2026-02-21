#pragma once

#include "cdc/sys/RCObject.h"

namespace cdc
{
	class ScriptType;

	class NativeScriptType : RCObject
	{
		const char* m_package;
		const char* m_name;
		int field_10;
		int field_14;
		int field_18;
		cdc::ScriptType* m_type;
	};
}