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
	};
}
