#pragma once

#include "ScriptType.h"

namespace cdc
{
	class ScriptObject
	{
	public:
		int field_0;
		int field_4;
		cdc::ScriptType* m_scriptType;
		char* m_instance;
		int field_10;
		int field_14;
		char m_updatingSequences;
		char field_19;
		__int16 field_1A;
	};
}