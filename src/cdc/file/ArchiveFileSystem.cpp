#include "ArchiveFileSystem.h"

#include "util/Hooking.h"

cdc::ArchiveFileSystem::ArchiveFileSystem(FileSystem* pDiskFS)
{
	// Call the original constructor
	Hooking::ThisCall(0x47E0D0, this, pDiskFS);
}

bool cdc::ArchiveFileSystem::Open(const char* archiveFileName)
{
	return Hooking::ThisCallReturn<bool>(0x47DDF0, this, archiveFileName);
}