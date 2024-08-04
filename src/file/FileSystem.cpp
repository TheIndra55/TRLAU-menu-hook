#include "FileSystem.h"
#include "FileReceivers.h"
#include "util/Hooking.h"

cdc::FileSystem* GetFS()
{
	auto addr = GET_ADDRESS(0x45C700, 0x45F640, 0x472B50);

	return Hooking::CallReturn<cdc::FileSystem*>(addr);
}

char* FSHelper_ReadFile(const char* fileName, cdc::FileSystem* fileSystem)
{
	auto size = fileSystem->GetFileSize(fileName);
	auto buffer = new char[size + 1];

	auto receiver = new FileUserBufferReceiver(buffer);
	auto request = fileSystem->RequestRead(receiver, fileName, 0);

	if (request)
	{
		request->Submit(cdc::FileRequest::HIGH);
		fileSystem->Synchronize();

		// Null terminate the buffer
		buffer[size] = 0;

		return buffer;
	}

	return nullptr;
}