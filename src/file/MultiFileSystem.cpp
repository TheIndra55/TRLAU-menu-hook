 #include "MultiFileSystem.h"

#include "Hook.h"
#include "modules/Log.h"

MultiFileSystem::MultiFileSystem() : m_fileSystems()
{
}

// Gets the best file system for a file by checking all file systems
cdc::FileSystem* MultiFileSystem::GetBestFileSystem(const char* fileName)
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

void MultiFileSystem::Add(cdc::FileSystem* fileSystem)
{
	m_fileSystems.push_back(fileSystem);

	Hook::GetInstance().GetModule<Log>()->WriteLine("Mounted new file system %p, number of systems: %d", fileSystem, m_fileSystems.size());
}

void MultiFileSystem::Remove(cdc::FileSystem* fileSystem)
{
	m_fileSystems.erase(std::remove(m_fileSystems.begin(), m_fileSystems.end(), fileSystem));
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

unsigned int MultiFileSystem::GetFileSize(char const* fileName)
{
	auto fileSystem = GetBestFileSystem(fileName);

	return fileSystem->GetFileSize(fileName);
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
	if (m_fileSystems.empty())
	{
		return 0xFFFFFFFF;
	}

	return m_fileSystems[0]->GetSpecialisationMask();
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