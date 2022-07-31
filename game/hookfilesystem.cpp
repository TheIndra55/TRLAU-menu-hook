#include "filesystem.hpp"

#include <string.h>
#include <ctype.h>

// so we don't need to include <Windows.h>
#define _MAX_PATH 260

class HookFileSystem : public cdc::FileSystem
{
private:
	cdc::FileSystem* m_pFS;
	cdc::FileSystem* m_pDiskFS;

public:
	HookFileSystem(cdc::FileSystem* pFS, cdc::FileSystem* pDiskFS)
	{
		m_pFS = pFS;
		m_pDiskFS = pDiskFS;
	}

	cdc::FileSystem* GetBestFileSystem(const char* fileName, char** outFilename)
	{
		// suffix pc-w to mods folder
		char path[_MAX_PATH];
		strcpy_s(path, fileName);

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
			memmove(path + 4, path, sizeof(path) - 4);
			strncpy(path, "mods", 4);
		}

		// check if file exists on disk, if so return the diskFS
		if (m_pDiskFS->FileExists(path))
		{
			*outFilename = path;
			return m_pDiskFS;
		}

		*outFilename = (char*)fileName;
		return m_pFS;
	}

	virtual void* RequestRead(void* receiver, const char* fileName, unsigned int startOffset)
	{
		char* path;
		auto pFS = GetBestFileSystem(fileName, &path);

		return pFS->RequestRead(receiver, path, startOffset);
	}

	virtual void* OpenFile(char const* fileName)
	{
		char* path;
		auto pFS = GetBestFileSystem(fileName, &path);

		return pFS->OpenFile(path);
	}

	virtual bool FileExists(char const* fileName)
	{
		char* path;
		auto pFS = GetBestFileSystem(fileName, &path);

		return pFS->FileExists(path);
	}

	virtual unsigned int GetFileSize(char const* fileName)
	{
		char* path;
		auto pFS = GetBestFileSystem(fileName, &path);

		return pFS->GetFileSize(path);
	}

	virtual void SetSpecialisationMask(unsigned int specMask)
	{
		m_pFS->SetSpecialisationMask(specMask);
	}

	virtual unsigned int GetSpecialisationMask()
	{
		return m_pFS->GetSpecialisationMask();
	}

	// these below don't need to call diskFS, the archive filesystem will do so
	virtual int GetStatus()
	{
		return m_pFS->GetStatus();
	}

	virtual void Update()
	{
		m_pFS->Update();
	}

	virtual void Synchronize()
	{
		m_pFS->Synchronize();
	}

#if TR8
	virtual void Suspend()
	{
		m_pFS->Suspend();
	}

	virtual bool Resume()
	{
		return m_pFS->Resume();
	}

	virtual bool IsSuspended()
	{
		return m_pFS->IsSuspended();
	}
#endif
};

cdc::FileSystem* CreateHookFileSystem(cdc::FileSystem* pFS, cdc::FileSystem* pDiskFS)
{
	return new HookFileSystem(pFS, pDiskFS);
}
