#pragma once

#include "Heap.h"

namespace cdc
{
	enum MemTag
	{
		kMemTagMisc,
		kMemTagDebug,
		kMemTagAlloc = 0xff
	};

	class Allocator
	{
	public:
		struct Overhead
		{
			int m_alloc;
			int m_allocDebug;
			int m_pool;
			int m_poolDebug;
		};

		virtual ~Allocator() = 0;
		virtual int GetType() = 0;

		virtual unsigned int GetAlignment() = 0;
		virtual unsigned int MaxAllocSize() = 0;
		virtual unsigned int GetPoolSize() = 0;
		virtual void GetOverhead(Overhead* overhead) = 0;
		virtual bool InAddressSpace(const void* ptr) = 0;
		virtual bool IsAllocation(const void* ptr) = 0;
		virtual bool IsAllocated(const void* ptr) = 0;
		virtual unsigned int PtrSize(const void* ptr) = 0;

		virtual bool CheckMemoryCorruption() = 0;
		virtual bool CheckMemoryCorruption(const void* ptr) = 0;
		virtual bool CheckMemoryLeaks() = 0;

		virtual void* Alloc(unsigned int size, MemTag tag) = 0;
		virtual void Free(void* ptr) = 0;
		virtual bool IsEmpty() = 0;
		virtual void* GetContext(void* prevContext) = 0;
		virtual void* GetPtr(void* context) = 0;
	};

	class HeapAllocator : public Allocator
	{
	public:
		virtual unsigned int GetMaxHeapSize() = 0;
		virtual unsigned int GetPageSize() = 0;
		virtual unsigned int GetTopAddress() = 0;
		virtual unsigned int GetTotalAllocSize() = 0;

		virtual void* AllocNoAbort(unsigned int size, MemTag tag) = 0;
		virtual void* AllocAlign(unsigned int align, unsigned int size, MemTag tag) = 0;
		virtual void* Realloc(void* ptr, unsigned int size) = 0;

		virtual Heap* GetHeapImpl() = 0;
	};
}