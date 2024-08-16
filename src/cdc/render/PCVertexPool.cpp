#include "PCVertexPool.h"
#include "util/Hooking.h"

cdc::PCDynamicVertexBuffer* cdc::PCBasicVertexPool::Create(unsigned int numVertices, unsigned int* baseOffsetInVerts)
{
	auto addr = GET_ADDRESS(0xECDE60, 0x61F840, 0x000000);

	return Hooking::ThisCallReturn<cdc::PCDynamicVertexBuffer*>(addr, this, numVertices, baseOffsetInVerts);
}