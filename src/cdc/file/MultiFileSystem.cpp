#include <Hooking.Patterns.h>

#include "MultiFileSystem.h"
#include "util/Hooking.h"

// Adds a file system to the multi file system
void cdc::MultiFileSystem::Add(FileSystem* fileSystem, bool bReprioritize, bool bAddToFront)
{
	auto match = hook::pattern("80 7C 24 0C 00 8B C1 74 5A 53").count(1);

	Hooking::ThisCall((unsigned int)match.get_first(), this, fileSystem, bReprioritize, bAddToFront);
}