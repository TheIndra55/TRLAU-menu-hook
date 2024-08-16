#include "TransientHeapAllocator.h"

char* cdc::TransientHeapAllocator::Alloc(unsigned int size)
{
	auto ptr = m_freePtr;

	if (ptr + size > m_size)
	{
		return nullptr;
	}

	m_freePtr += size;
	return &m_pHeap[ptr];
}