#include "MultiFileSystem.h"

#include "Hook.h"
#include "modules/Log.h"

MultiFileSystem::MultiFileSystem() : m_fileSystems()
{
}

void MultiFileSystem::Add(cdc::FileSystem* fileSystem)
{
	m_fileSystems.push_back(fileSystem);

#ifdef _DEBUG
	Hook::GetInstance().GetModule<Log>()->WriteLine("Mounted new file system %p, number of systems: %d", fileSystem, m_fileSystems.size());
#endif
}

void MultiFileSystem::Remove(cdc::FileSystem* fileSystem)
{
}

cdc::FileSystem* MultiFileSystem::GetBestFileSystem(const char* fileName) const noexcept
{
	for (auto fileSystem : m_fileSystems)
	{
		if (fileSystem->FileExists(fileName))
		{
			return fileSystem;
		}
	}

	return nullptr;
}

cdc::FileRequest* MultiFileSystem::RequestRead(cdc::FileReceiver* receiver, const char* fileName, unsigned int startOffset)
{
	auto fileSystem = GetBestFileSystem(fileName);

	return fileSystem->RequestRead(receiver, fileName, startOffset);
}

cdc::File* MultiFileSystem::OpenFile(char const* fileName)
{
	auto fileSystem = GetBestFileSystem(fileName);

	return fileSystem->OpenFile(fileName);
}

unsigned int MultiFileSystem::GetFileSize(char const* fileName)
{
	auto fileSystem = GetBestFileSystem(fileName);

	return fileSystem->GetFileSize(fileName);
}

bool MultiFileSystem::FileExists(char const* fileName)
{
	for (auto fileSystem : m_fileSystems)
	{
		if (fileSystem->FileExists(fileName))
		{
			return true;
		}
	}

	return false;
}

void MultiFileSystem::SetSpecialisationMask(unsigned int specMask)
{
	for (auto fileSystem : m_fileSystems)
	{
		fileSystem->SetSpecialisationMask(specMask);
	}
}

unsigned int MultiFileSystem::GetSpecialisationMask()
{
	return m_fileSystems.empty() ? 0xFFFFFFFF : m_fileSystems[0]->GetSpecialisationMask();
}

cdc::FileSystem::Status MultiFileSystem::GetStatus()
{
	for (auto fileSystem : m_fileSystems)
	{
		if (fileSystem->GetStatus() == BUSY)
		{
			return BUSY;
		}
	}

	return IDLE;
}

void MultiFileSystem::Update()
{
	for (auto fileSystem : m_fileSystems)
	{
		fileSystem->Update();
	}
}

void MultiFileSystem::Synchronize()
{
	for (auto fileSystem : m_fileSystems)
	{
		fileSystem->Synchronize();
	}
}