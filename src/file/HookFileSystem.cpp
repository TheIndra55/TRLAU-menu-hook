#include <Windows.h>
#include <stdio.h>

#include "HookFileSystem.h"
#include "Hook.h"
#include "modules/Log.h"

HookFileSystem::HookFileSystem(cdc::FileSystem* pDiskFS)
{
	m_pDiskFS = pDiskFS;
	m_specMask = 1;
}

// Finds a file in the mods folder and rewrites the path
bool HookFileSystem::FindFile(const char* fileName, char* path)
{
	strcpy_s(path, _MAX_PATH, fileName);

	for (char* p = path; *p; ++p)
		*p = tolower(*p);

	// Rewrite pc-w to mods folder
	if (strncmp(path, "pc-w", 4) == 0)
	{
		strncpy(path, "mods", 4);
	}

	// Rewrite root to mods folder
	if (strncmp(path, "\\", 1) == 0)
	{
		// Move string 4 bytes
		memmove(path + 4, path, _MAX_PATH - 4);
		strncpy(path, "mods", 4);
	}

	// Check first for file on disk suffixed by our specialisation mask
	char specPath[_MAX_PATH];
	sprintf_s(specPath, "%s_%03d", path, m_specMask);

	if (m_pDiskFS->FileExists(specPath))
	{
		strcpy_s(path, _MAX_PATH, specPath);

		return true;
	}

	// Check if file exists on disk, if so return the diskFS
	if (m_pDiskFS->FileExists(path))
	{
		return true;
	}

	return false;
}

cdc::FileRequest* HookFileSystem::RequestRead(cdc::FileReceiver* receiver, const char* fileName, unsigned int startOffset)
{
	char path[_MAX_PATH];
	FindFile(fileName, path);

	Hook::GetInstance().GetModule<Log>()->WriteLine("Loading %s from mods folder", fileName);

	return m_pDiskFS->RequestRead(receiver, path, startOffset);
}

cdc::File* HookFileSystem::OpenFile(char const* fileName)
{
	char path[_MAX_PATH];
	FindFile(fileName, path);

	return m_pDiskFS->OpenFile(path);
}

bool HookFileSystem::FileExists(char const* fileName)
{
	char path[_MAX_PATH];
	return FindFile(fileName, path);
}

unsigned int HookFileSystem::GetFileSize(char const* fileName)
{
	char path[_MAX_PATH];
	FindFile(fileName, path);

	return m_pDiskFS->GetFileSize(path);
}

void HookFileSystem::SetSpecialisationMask(unsigned int specMask)
{
	m_pDiskFS->SetSpecialisationMask(specMask);

	// Unset next generation bit and set our spec mask
	m_specMask = specMask & ~0x80000000;
}

unsigned int HookFileSystem::GetSpecialisationMask()
{
	return 0;
}

cdc::FileSystem::Status HookFileSystem::GetStatus()
{
	return m_pDiskFS->GetStatus();
}

void HookFileSystem::Update()
{
	m_pDiskFS->Update();
}

void HookFileSystem::Synchronize()
{
	m_pDiskFS->Synchronize();
}

#ifdef TR8
void HookFileSystem::Suspend()
{
	m_pDiskFS->Suspend();
}

bool HookFileSystem::Resume()
{
	return m_pDiskFS->Resume();
}

bool HookFileSystem::IsSuspended()
{
	return m_pDiskFS->IsSuspended();
}

char* HookFileSystem::GetBufferPointer(cdc::FileRequest* request, unsigned int* bytesLocked)
{
	return m_pDiskFS->GetBufferPointer(request, bytesLocked);
}

void HookFileSystem::ResetBufferPointer(int value)
{
	m_pDiskFS->ResetBufferPointer(value);
}

#endif