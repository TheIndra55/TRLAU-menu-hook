#include "MultiFileSystem.h"

MultiFileSystem::MultiFileSystem(cdc::FileSystem* pFS, cdc::FileSystem* pHookFS)
{
	m_pFS = pFS;
	m_pHookFS = pHookFS;
}

// Gets the best file system for a file simply by checking the hook file system first
cdc::FileSystem* MultiFileSystem::GetBestFileSystem(const char* fileName)
{
	// First check the hook file system, else fall back to default filesystem
	if (m_pHookFS->FileExists(fileName))
	{
		return m_pHookFS;
	}

	return m_pFS;
}

cdc::FileRequest* MultiFileSystem::RequestRead(cdc::FileReceiver* receiver, const char* fileName, unsigned int startOffset)
{
	auto pFS = GetBestFileSystem(fileName);

	return pFS->RequestRead(receiver, fileName, startOffset);
}

cdc::File* MultiFileSystem::OpenFile(char const* fileName)
{
	auto pFS = GetBestFileSystem(fileName);

	return pFS->OpenFile(fileName);
}

bool MultiFileSystem::FileExists(char const* fileName)
{
	return m_pFS->FileExists(fileName) || m_pHookFS->FileExists(fileName);
}

unsigned int MultiFileSystem::GetFileSize(char const* fileName)
{
	auto pFS = GetBestFileSystem(fileName);

	return pFS->GetFileSize(fileName);
}

void MultiFileSystem::SetSpecialisationMask(unsigned int specMask)
{
	m_pFS->SetSpecialisationMask(specMask);
	m_pHookFS->SetSpecialisationMask(specMask);
}

unsigned int MultiFileSystem::GetSpecialisationMask()
{
	return m_pFS->GetSpecialisationMask();
}

// These only need to call the default file system, both will end at the same place

cdc::FileSystem::Status MultiFileSystem::GetStatus()
{
	return m_pFS->GetStatus();
}

void MultiFileSystem::Update()
{
	m_pFS->Update();
}

void MultiFileSystem::Synchronize()
{
	m_pFS->Synchronize();
}