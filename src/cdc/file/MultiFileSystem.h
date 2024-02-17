#pragma once

#include "FileSystem.h"

namespace cdc
{
	class MultiFileSystem : public cdc::FileSystem
	{
	public:
		void Add(FileSystem* fileSystem, bool bReprioritize, bool bAddToFront);
	};
}