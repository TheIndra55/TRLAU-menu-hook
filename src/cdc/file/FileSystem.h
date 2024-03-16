#pragma once

namespace cdc
{
	class FileRequest
	{
	};

	class FileReceiver
	{
	public:
		virtual int ReceiveData(const char* data, unsigned int dataSize, unsigned int requestOffset) = 0;
		virtual void ReceiveStarted(FileRequest* request, unsigned int requestSize) = 0;
		virtual void ReceiveCancelled(FileRequest* request) = 0;
		virtual void ReceiveDone(FileRequest* request) = 0;
	};

	class File
	{
	};

	class FileSystem
	{
	public:
		enum Status
		{
			IDLE,
			BUSY
		};

		virtual FileRequest* RequestRead(FileReceiver* receiver, const char* fileName, unsigned int startOffset) = 0;
		virtual File* OpenFile(const char* fileName) = 0;
		virtual bool FileExists(const char* fileName) = 0;
		virtual unsigned int GetFileSize(const char* fileName) = 0;
		virtual void SetSpecialisationMask(unsigned int specMask) = 0;
		virtual unsigned int GetSpecialisationMask() = 0;
		virtual Status GetStatus() = 0;
		virtual void Update() = 0;
		virtual void Synchronize() = 0;

#ifdef TR8
		virtual void Suspend() = 0;
		virtual bool Resume() = 0;
		virtual bool IsSuspended() = 0;
		virtual char* GetBufferPointer(FileRequest* request, unsigned int* bytesLocked) = 0;
		virtual void ResetBufferPointer(int value) = 0;
#endif

		virtual ~FileSystem() { };
	};
}