#pragma once

namespace cdc
{
	class TransientHeapAllocator
	{
	private:
		char* m_pHeap;
		unsigned int m_size;
		unsigned int m_freePtr;
		
	public:
		char* Alloc(unsigned int size);
	};
}