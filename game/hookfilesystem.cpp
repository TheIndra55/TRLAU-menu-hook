#include "filesystem.hpp"

#include <string.h>
#include <ctype.h>
#include <stdio.h>

// so we don't need to include <Windows.h>
#define _MAX_PATH 260

// the HookFileSystem will look for a file in the mods folder and rewrite the path
// for the disk filesystem to open, allowing it to load files without a bigfile

class HookFileSystem : public cdc::FileSystem
{
private:
	cdc::FileSystem* m_pDiskFS;

	unsigned int m_specMask;

public:
	HookFileSystem(cdc::FileSystem* pDiskFS)
	{
		m_pDiskFS = pDiskFS;
		m_specMask = 1;
	}

	// finds a file in the mods folder and rewrites path to it
	bool FindFile(const char* fileName, char* path)
	{
		strcpy_s(path, _MAX_PATH, fileName);

		for (char* p = path; *p; ++p) *p = tolower(*p);

		// rewrite pc-w to mods folder
		if (strncmp(path, "pc-w", 4) == 0)
		{
			strncpy(path, "mods", 4);
		}

		// rewrite root to mods folder
		if (strncmp(path, "\\", 1) == 0)
		{
			// move string 4 bytes
			memmove(path + 4, path, _MAX_PATH - 4);
			strncpy(path, "mods", 4);
		}

		// check first for file on disk suffixed by our specialisation mask
		char specPath[_MAX_PATH];
		sprintf_s(specPath, "%s_%03d", path, m_specMask);

		if (m_pDiskFS->FileExists(specPath))
		{
			strcpy_s(path, _MAX_PATH, specPath);

			return true;
		}

		// check if file exists on disk, if so return the diskFS
		if (m_pDiskFS->FileExists(path))
		{
			return true;
		}

		return false;
	}

	virtual void* RequestRead(void* receiver, const char* fileName, unsigned int startOffset)
	{
		char path[_MAX_PATH];
		FindFile(fileName, path);

		return m_pDiskFS->RequestRead(receiver, path, startOffset);
	}

	virtual void* OpenFile(char const* fileName)
	{
		char path[_MAX_PATH];
		FindFile(fileName, path);

		return m_pDiskFS->OpenFile(path);
	}

	virtual bool FileExists(char const* fileName)
	{
		char path[_MAX_PATH];
		return FindFile(fileName, path);
	}

	virtual unsigned int GetFileSize(char const* fileName)
	{
		char path[_MAX_PATH];
		FindFile(fileName, path);

		return m_pDiskFS->GetFileSize(path);
	}

	virtual void SetSpecialisationMask(unsigned int specMask)
	{
		m_pDiskFS->SetSpecialisationMask(specMask);

		// unset next generation bit and set our spec mask
		m_specMask = specMask & ~0x80000000;
	}

	virtual unsigned int GetSpecialisationMask()
	{
		return 0;
	}

	virtual int GetStatus()
	{
		return m_pDiskFS->GetStatus();
	}

	virtual void Update()
	{
		m_pDiskFS->Update();
	}

	virtual void Synchronize()
	{
		m_pDiskFS->Synchronize();
	}

#if TR8
	virtual void Suspend()
	{
		m_pDiskFS->Suspend();
	}

	virtual bool Resume()
	{
		return m_pDiskFS->Resume();
	}

	virtual bool IsSuspended()
	{
		return m_pDiskFS->IsSuspended();
	}
#endif
};

cdc::FileSystem* CreateHookFileSystem(cdc::FileSystem* pDiskFS)
{
	return new HookFileSystem(pDiskFS);
}
