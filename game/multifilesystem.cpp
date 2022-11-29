#include "filesystem.hpp"

// very simplified version of cdc::MultiFileSystem since it does not exist in Legend and Anniversary
// the real MultiFileSystem can dynamically add and re-order filesystems

class MultiFileSystem : public cdc::FileSystem
{
private:
	cdc::FileSystem* m_pFS;
	cdc::FileSystem* m_pHookFS;

public:
	MultiFileSystem(cdc::FileSystem* pFS, cdc::FileSystem* pHookFS)
	{
		m_pFS = pFS;
		m_pHookFS = pHookFS;
	}

	cdc::FileSystem* GetBestFileSystem(const char* fileName)
	{
		// first check hook filesystem, else fall back to default filesystem
		if (m_pHookFS->FileExists(fileName))
		{
			return m_pHookFS;
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
		return m_pFS->FileExists(fileName) || m_pHookFS->FileExists(fileName);
	}

	virtual unsigned int GetFileSize(char const* fileName)
	{
		auto pFS = GetBestFileSystem(fileName);

		return pFS->GetFileSize(fileName);
	}

	virtual void SetSpecialisationMask(unsigned int specMask)
	{
		m_pFS->SetSpecialisationMask(specMask);
		m_pHookFS->SetSpecialisationMask(specMask);
	}

	virtual unsigned int GetSpecialisationMask()
	{
		return m_pFS->GetSpecialisationMask();
	}

	// only need to call the default filesystem, both will end at the same place
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

#ifndef TR8
cdc::FileSystem* CreateMultiFileSystem(cdc::FileSystem* pFS, cdc::FileSystem* pDiskFS)
{
	auto pHookFS = CreateHookFileSystem(pDiskFS);

	return new MultiFileSystem(pFS, pHookFS);
}
#endif
