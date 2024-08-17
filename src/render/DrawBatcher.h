#pragma once

#include <vector>

#include "Draw.h"

// Batcher for DRAW_ calls to limit the amount of draw calls sent to the GPU
class DrawBatcher
{
private:
	std::vector<DRAWVERTEX> m_vertices;

	static DrawBatcher* s_batcher;

public:
	DrawBatcher();

	void DrawTriangles(DRAWVERTEX* verts, int numtris) noexcept;
	void Flush() noexcept;

	static DrawBatcher* GetInstance();
};