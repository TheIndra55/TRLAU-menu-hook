#pragma once

#include "cdc/file/FileSystem.h"

// So we don't need to include Windows.h here
#define _MAX_PATH 260

// This file system represents a subfolder in the mods oflder
class ModFileSystem : public cdc::FileSystem
{
private:
	cdc::FileSystem* m_diskSystem;

	unsigned int m_specMask;
	unsigned int m_langMask;

	char m_basePath[_MAX_PATH];
	char m_path[_MAX_PATH];

	// Finds a file and returns whether it exists and the rewritten path
	bool FindFile(const char* fileName, char* path) const noexcept;

public:
	ModFileSystem(const char* path);

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
	char* GetBufferPointer(cdc::FileRequest* request, unsigned int* bytesLocked);
	void ResetBufferPointer(int value);
#endif
};