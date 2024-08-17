#include "DrawBatcher.h"

DrawBatcher* DrawBatcher::s_batcher = nullptr;

DrawBatcher::DrawBatcher() : m_vertices()
{
	m_vertices.reserve(0x2000);
}

void DrawBatcher::DrawTriangles(DRAWVERTEX* verts, int numtris) noexcept
{
	auto numVerts = numtris * 3;

	// Prevent allocations
	if (m_vertices.size() + numVerts >= m_vertices.capacity())
	{
		Flush();
	}

	// Add to batch
	for (int i = 0; i < numVerts; i++)
	{
		m_vertices.push_back(verts[i]);
	}
}

void DrawBatcher::Flush() noexcept
{
	if (m_vertices.size() > 0)
	{
		DRAW_DrawTriangles(2, 0, m_vertices.data(), m_vertices.size() / 3);

		m_vertices.clear();
	}
}

DrawBatcher* DrawBatcher::GetInstance()
{
	// Since this is hot code which is called a lot during a frame
	// there's a manual singleton here instead of a C++11 standard one
	// which uses thread local storage
	if (s_batcher == nullptr)
	{
		s_batcher = new DrawBatcher();
	}

	return s_batcher;
}

#ifdef BATCH_DRAW_CALLS
void DrawTriangles(DRAWVERTEX* verts, int numtris)
{
	DrawBatcher::GetInstance()->DrawTriangles(verts, numtris);
}
#endif