#pragma once

#include <d3d9.h>

#include "TransientHeapAllocator.h"

namespace cdc
{
	class PCVertexFormat
	{
	};

	class PCVertexBufferMemPool
	{
	};

	class PCVertexBuffer
	{
	public:
		virtual ~PCVertexBuffer() = 0;

		virtual unsigned int GetBaseVertexIndex() = 0;
		virtual IDirect3DVertexBuffer9* GetD3DVertexBuffer() = 0;
		virtual IDirect3DVertexDeclaration9* GetD3DVertexDeclaration() = 0;
		virtual unsigned int GetNumVertices() = 0;
		virtual unsigned __int16 GetStride() = 0;
	};

	class PCDynamicVertexBuffer : public PCVertexBuffer
	{
	public:
		IDirect3DVertexBuffer9* m_pD3DVertexBuffer;
		void* m_pVertexData;
		PCVertexFormat* m_pVertexFormat;
		unsigned int m_baseVertexIndex;
		unsigned int m_numVertices;
	};

	class PCBasicVertexPool
	{
	private:
		PCVertexFormat* m_pVertexFormat;
		cdc::PCVertexBufferMemPool* m_pVertexBufferMemPool;
		TransientHeapAllocator* m_pFrameHeap;
		IDirect3DVertexBuffer9* m_pLastVB;
		cdc::PCDynamicVertexBuffer* m_pLastSharedVB;
		unsigned int m_nVertices;

	public:
		PCDynamicVertexBuffer* Create(unsigned int numVertices, unsigned int* baseOffsetInVerts);

		virtual ~PCBasicVertexPool() { };
	};
}