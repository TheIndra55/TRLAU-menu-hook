#include "PCPrimitivePool.h"
#include "util/Hooking.h"

void* cdc::PCPrimitivePool::AllocateVertices(unsigned int numVerts)
{
	unsigned int baseOffset;
	auto vertices = m_vertexPool.Create(numVerts, &baseOffset);
	auto offset = 0;

	auto currentPrimitive = m_currentPrimitive->currentPrimitive;

	if (currentPrimitive && currentPrimitive->vertexBuffer == vertices)
	{
		offset = currentPrimitive->numVertices * m_stride;
		currentPrimitive->numVertices += numVerts;
	}
	else
	{
		auto primitive = (PartialPrimitive*)m_pFrameHeap->Alloc(sizeof(PartialPrimitive));

		primitive->baseOffset = baseOffset;
		primitive->next = nullptr;
		primitive->numVertices = numVerts;
		primitive->vertexBuffer = vertices;

		if (m_currentPrimitive->currentPrimitive)
		{
			m_currentPrimitive->currentPrimitive->next = primitive;
		}
		else
		{
			m_currentPrimitive->firstPrimitive = primitive;
		}

		m_currentPrimitive->currentPrimitive = primitive;
	}

	m_currentPrimitive->totalNumVertices += numVerts;
	m_totalNumVertices += numVerts;

	return (char*)vertices->m_pVertexData + offset;
}