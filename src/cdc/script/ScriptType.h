#pragma once

#include "cdc/sys/RCObject.h"
#include "cdc/sys/SArray.h"
#include "NativeScriptType.h"
#include "DataType.h"
#include "ScriptName.h"

namespace cdc
{
	class StartFunctionInfo;

	class DataMember
	{
	public:
		DataType m_type;
		unsigned __int16 m_offset;
		Symbol m_name;
		int m_init;
	};

	class Prototype
	{
	public:
		ScriptType* m_script;

		unsigned __int8 m_flags;
		unsigned __int8 m_callType;
		unsigned __int16 m_vtIndex;

		ScriptName m_name;
		SArray<cdc::DataMember> m_args;
		DataType m_retType;
	};

	class Function
	{
	public:
		Prototype* m_prototype;

		unsigned __int8 m_flags;
		unsigned __int8 m_state;
		unsigned __int16 m_size;
		unsigned __int16 m_align;
		unsigned __int16 m_stackSize;

		SArray<DataMember> m_locals;
		SArray<StartFunctionInfo> m_startFunctionList;
		int field_14;
		int field_18;
	};

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

		SArray<ScriptName> m_states; // 20
		SArray<EnumDecl> m_enums; // 24
		SArray<StructDecl> m_structs; // 28
		SArray<DataMember> m_members; // 2C
		SArray<StartFunctionInfo> m_startFunctionList; // 30
		SArray<Prototype> m_prototypes; // 34
		SArray<Function> m_functions; // 38
		int field_3C; // 3C
		int field_40; // 40
		int field_44; // 44
		SArray<ScriptType*> m_dependencies; // 48
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