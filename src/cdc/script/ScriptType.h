#pragma once

#include "cdc/sys/RCObject.h"
#include "NativeScriptType.h"
#include "ScriptName.h"

namespace cdc
{
	class ScriptType;

	class ScriptTypeStreamData
	{
	public:
		int version;
		int field_4;
		ScriptName m_name;
		const char* m_nativeScriptPackageName;
		const char* m_nativeScriptName;
		ScriptType* m_scriptType;
		ScriptType* m_superScriptType;
		unsigned __int16 m_size;
		unsigned __int16 m_align;
	};

	class ScriptType : RCObject
	{
	public:
		ScriptTypeStreamData* m_streamData;
		NativeScriptType* m_native;
		int field_10;
		int field_14;
		int field_18;
	};
}