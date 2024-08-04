#include "FileReceivers.h"
#include <cstring>

FileUserBufferReceiver::FileUserBufferReceiver(char* buffer) : m_buffer(buffer)
{
}

int FileUserBufferReceiver::ReceiveData(const char* data, unsigned int dataSize, unsigned int requestOffset)
{
	memcpy(&m_buffer[requestOffset], data, dataSize);
	return dataSize;
}

void FileUserBufferReceiver::ReceiveStarted(cdc::FileRequest* request, unsigned int requestSize)
{
}

void FileUserBufferReceiver::ReceiveCancelled(cdc::FileRequest* request)
{
}

void FileUserBufferReceiver::ReceiveDone(cdc::FileRequest* request)
{
}