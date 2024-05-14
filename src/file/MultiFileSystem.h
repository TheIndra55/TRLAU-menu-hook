#pragma once

#include <vector>

#include "cdc/file/FileSystem.h"

class MultiFileSystem : public cdc::FileSystem
{
private:
	std::vector<FileSystem*> m_fileSystems;

	cdc::FileSystem* GetBestFileSystem(const char* fileName);
public:
	MultiFileSystem();
	void Add(cdc::FileSystem* fileSystem);

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
	void Suspend() { }
	bool Resume() { return false; }
	bool IsSuspended() { return false; }
	char* GetBufferPointer(cdc::FileRequest* request, unsigned int* bytesLocked) { return nullptr; }
	void ResetBufferPointer(int value) { }
#endif
};