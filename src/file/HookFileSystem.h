#pragma once

#include "cdc/file/FileSystem.h"

// This file system will read files from the mods folder in the game directory
// the path will be rewritten and passed to the native file system
class HookFileSystem : public cdc::FileSystem
{
private:
	cdc::FileSystem* m_pDiskFS;
	unsigned int m_specMask;

	bool FindFile(const char* fileName, char* path);
public:
	HookFileSystem(cdc::FileSystem* pDiskFS);

	cdc::FileRequest* RequestRead(cdc::FileReceiver* receiver, const char* fileName, unsigned int startOffset);
	cdc::File* OpenFile(const char* fileName);
	bool FileExists(const char* fileName);
	unsigned int GetFileSize(const char* fileName);
	void SetSpecialisationMask(unsigned int specMask);
	unsigned int GetSpecialisationMask();
	Status GetStatus();
	void Update();
	void Synchronize();

#ifdef TR8
	void Suspend();
	bool Resume();
	bool IsSuspended();
#endif
};