#include "filesystem.hpp"

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

	cdc::FileSystem* GetBestFileSystem(const char* fileName)
	{
		// check if file exists on disk, if so return the diskFS
		if (m_pDiskFS->FileExists(fileName))
		{
			return m_pDiskFS;
		}

		return m_pFS;
	}

	virtual void* RequestRead(void* receiver, const char* fileName, unsigned int startOffset)
	{
		auto pFS = GetBestFileSystem(fileName);

		return pFS->RequestRead(receiver, fileName, startOffset);
	}

	virtual void* OpenFile(char const* fileName)
	{
		auto pFS = GetBestFileSystem(fileName);

		return pFS->OpenFile(fileName);
	}

	virtual bool FileExists(char const* fileName)
	{
		return (m_pFS->FileExists(fileName) || m_pDiskFS->FileExists(fileName));
	}

	virtual unsigned int GetFileSize(char const* fileName)
	{
		auto pFS = GetBestFileSystem(fileName);

		return pFS->GetFileSize(fileName);
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
};

cdc::FileSystem* CreateHookFileSystem(cdc::FileSystem* pFS, cdc::FileSystem* pDiskFS)
{
	return new HookFileSystem(pFS, pDiskFS);
}
