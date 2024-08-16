#pragma once

#include <d3d9.h>

#include "TransientHeapAllocator.h"
#include "PCVertexPool.h"

namespace cdc
{
	class PCPrimitivePool
	{
	private:
		TransientHeapAllocator* m_pFrameHeap;

		PCBasicVertexPool m_vertexPool;
		D3DPRIMITIVETYPE m_primType;

		unsigned int m_primVerts;
		unsigned int m_totalNumVertices;
		unsigned int m_stride;

		struct PartialPrimitive
		{
			PCDynamicVertexBuffer* vertexBuffer;
			unsigned int numVertices;
			unsigned int baseOffset;
			PartialPrimitive* next;
		};

		struct Primitive
		{
			PartialPrimitive* firstPrimitive;
			PartialPrimitive* currentPrimitive;
			unsigned int totalNumVertices;
		};

		Primitive* m_currentPrimitive;

	public:
		void* AllocateVertices(unsigned int numVerts);
	};
}