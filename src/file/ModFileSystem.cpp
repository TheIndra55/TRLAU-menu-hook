#include "ModFileSystem.h"
#include "Hook.h"

#include "modules/Log.h"
#include "file/FileSystem.h"

ModFileSystem::ModFileSystem(const char* path) : m_specMask(1), m_langMask(1), m_path()
{
	m_diskSystem = GetDiskFS();

	strcpy(m_basePath, path);
}

bool ModFileSystem::FindFile(const char* fileName, char* path) const noexcept
{
	return false;
}

cdc::FileRequest* ModFileSystem::RequestRead(cdc::FileReceiver* receiver, const char* fileName, unsigned int startOffset)
{
	FindFile(fileName, m_path);

	Hook::GetInstance().GetModule<Log>()->WriteLine("Loading %s from mods folder", fileName);

	return m_diskSystem->RequestRead(receiver, m_path, startOffset);
}

cdc::File* ModFileSystem::OpenFile(const char* fileName)
{
	FindFile(fileName, m_path);

	return m_diskSystem->OpenFile(m_path);
}

bool ModFileSystem::FileExists(const char* fileName)
{
	return FindFile(fileName, m_path);
}

unsigned int ModFileSystem::GetFileSize(const char* fileName)
{
	FindFile(fileName, m_path);

	return m_diskSystem->GetFileSize(m_path);
}

void ModFileSystem::SetSpecialisationMask(unsigned int specMask)
{
	m_specMask = specMask;

	// Unset extra bit and set our language mask
	m_langMask = specMask & ~0x80000000;
}

unsigned int ModFileSystem::GetSpecialisationMask()
{
	return m_specMask;
}

cdc::FileSystem::Status ModFileSystem::GetStatus()
{
	return m_diskSystem->GetStatus();
}

void ModFileSystem::Update()
{
	m_diskSystem->Update();
}

void ModFileSystem::Synchronize()
{
	m_diskSystem->Synchronize();
}