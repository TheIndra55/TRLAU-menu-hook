namespace cdc
{
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