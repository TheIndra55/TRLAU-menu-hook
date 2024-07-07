#include "Allocator.h"
#include "util/Hooking.h"

cdc::Allocator* cdc::Alloc::Get()
{
	return *(cdc::Allocator**)GET_ADDRESS(0x1159DBC, 0x8AFEA4, 0x000000);
}

cdc::HeapAllocator* cdc::Alloc::GetHeap()
{
	return *(cdc::HeapAllocator**)GET_ADDRESS(0x1159DC0, 0x8AFEA8, 0x000000);
}