#pragma once

#include "FileSystem.h"

namespace cdc
{
	class ArchiveFileSystem : public FileSystem
	{
	public:
		ArchiveFileSystem(FileSystem* pDiskFS);

		bool Open(const char* archiveFileName);
	};
}