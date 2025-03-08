#pragma once

#include "cdc/file/FileSystem.h"

// Simplified version for cdc::MultiFileSystem since it does not exist in Legend and Anniversary
// The real MultiFileSystem can dynamically add and re-order systems
// 
// This just checks the hook file system first and else falls back to the
// original file system
class MultiFileSystem : public cdc::FileSystem
{
private:
	cdc::FileSystem* m_pFS;
	cdc::FileSystem* m_pHookFS;

	cdc::FileSystem* GetBestFileSystem(const char* fileName);
public:
	MultiFileSystem(cdc::FileSystem* pFS, cdc::FileSystem* pHookFS);

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