#pragma once

#include "FileSystem.h"

namespace cdc
{
	class MultiFileSystem : public FileSystem
	{
	public:
		void Add(FileSystem* fileSystem, bool bReprioritize, bool bAddToFront);
	};
}