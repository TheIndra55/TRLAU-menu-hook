#pragma once

namespace cdc
{
	class FileSystem
	{
	public:
		virtual void* RequestRead(void* receiver, const char* name, unsigned int startOffset) = 0;
		virtual void* OpenFile(char const* fileName) = 0;
		virtual bool FileExists(char const* fileName) = 0;
		virtual unsigned int GetFileSize(char const* fileName) = 0;
		virtual void SetSpecialisationMask(unsigned int specMask) = 0;
		virtual unsigned int GetSpecialisationMask() = 0;
		virtual int GetStatus() = 0;
		virtual void Update() = 0;
		virtual void Synchronize() = 0;
#if TR8
		virtual void Suspend() = 0;
		virtual bool Resume() = 0;
		virtual bool IsSuspended() = 0;
#endif
	};

	struct MSFileSystem
	{
		struct Request
		{
			char pad1[20];
			char m_pFileName[128];
			char pad2[20];
			unsigned int m_bytesRead;
			unsigned int m_bytesProcessed;
			int m_readState;
			unsigned int m_offset;
			unsigned int m_size;
			Request* m_next;
		};

		char pad[1099812];
		Request* m_queue;
		Request* m_free;
		unsigned int m_numUsedRequests;
	};
}

#define g_pDiskFS VAR_U_(DISKFS, cdc::MSFileSystem*)
#define g_pFS VAR_U_(ARCHIVEFS, cdc::MSFileSystem*)

cdc::FileSystem* GetFS();
cdc::FileSystem* CreateHookFileSystem(cdc::FileSystem* pFS, cdc::FileSystem* pDiskFS);