#pragma once

#include "Symbol.h"

namespace cdc
{
	class ScriptType;
	class EnumDecl;
	class StructDecl;

	class DataType
	{
	public:
		enum BaseType
		{
			DTVOID,
			BOOL,
			INT,
			FLOAT,
			STRING,
			STATE,
			ARRAY,
			DYNARRAY,
			MAP,
			UNKNOWN,
			NATIVE,
			SCRIPT,
			STRUCT,
			STRUCT_REF,
			ENUM,
			NTENUM
		};

		struct Compound
		{
			ScriptType* m_script;
			DataType* m_subType;
		};

		unsigned __int32 m_type : 4;
		unsigned __int32 m_resolved : 1;
		unsigned __int32 m_compiled : 1;
		unsigned __int32 m_isGenericArg : 1;
		unsigned __int32 m_isCompound : 1;
		unsigned __int32 m_arraySize : 8;

		Symbol m_userType;

		union {
			DataType* m_subType;
			EnumDecl* m_enum;
			StructDecl* m_struct;
			ScriptType* m_script;
			Compound* m_compound;
		};
	};
}