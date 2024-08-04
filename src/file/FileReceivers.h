#pragma once

#include "cdc/file/FileSystem.h"

class FileUserBufferReceiver : public cdc::FileReceiver
{
private:
	char* m_buffer;

public:
	FileUserBufferReceiver(char* buffer);

	int ReceiveData(const char* data, unsigned int dataSize, unsigned int requestOffset);
	void ReceiveStarted(cdc::FileRequest* request, unsigned int requestSize);
	void ReceiveCancelled(cdc::FileRequest* request);
	void ReceiveDone(cdc::FileRequest* request);
};