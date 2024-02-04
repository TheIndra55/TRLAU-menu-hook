#pragma once

#include "cdc/math/Vector.h"

struct DRAWVERTEX
{
	float x;
	float y;
	float z;

	unsigned int color;

	float u;
	float v;

	unsigned int anPad[2];
};

#undef RGB

#define RGBA(r, g, b, a) r | (g << 8) | (b << 16) | (a << 24)
#define RGB(r, g, b) r | (g << 8) | (b << 16) | (255 << 24)

void TRANS_TransToDrawVertexV4f(DRAWVERTEX* v, cdc::Vector3* vec);
void TRANS_RotTransPersVectorf(cdc::Vector3* srcvector, cdc::Vector3* dstvector);

void DRAW_DrawQuads(int flags, int tpage, DRAWVERTEX* verts, int numquads);
void DRAW_DrawTriangles(int flags, int tpage, DRAWVERTEX* verts, int numtris);

void DrawTriangle(cdc::Vector3* v0, cdc::Vector3* v1, cdc::Vector3* v2, int color);
void DrawPlane(cdc::Vector3* v0, cdc::Vector3* v1, int color);