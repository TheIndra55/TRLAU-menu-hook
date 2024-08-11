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

struct LINEVERTEX
{
	float x;
	float y;
	float z;

	unsigned int color;
};

#undef RGB

#define RGBA(r, g, b, a) r | (g << 8) | (b << 16) | (a << 24)
#define RGB(r, g, b) r | (g << 8) | (b << 16) | (255 << 24)

void TRANS_TransToDrawVertexV4f(DRAWVERTEX* v, cdc::Vector3* vec);
void TRANS_RotTransPersVectorf(cdc::Vector3* srcvector, cdc::Vector3* dstvector);
void TRANS_TransToDrawVertex(cdc::Vector3* vec, DRAWVERTEX* v);
void TRANS_TransToLineVertex(cdc::Vector3* vec, LINEVERTEX* v);

void DRAW_DrawQuads(int flags, int tpage, DRAWVERTEX* verts, int numquads);
void DRAW_DrawTriangles(int flags, int tpage, DRAWVERTEX* verts, int numtris);
void DRAW_DrawLines(LINEVERTEX* verts, int numlines);

void TransformToVertex(DRAWVERTEX* v, cdc::Vector3* vec);

void DrawTriangle(cdc::Vector3* v0, cdc::Vector3* v1, cdc::Vector3* v2, int color);
void DrawPlane(cdc::Vector3* v0, cdc::Vector3* v1, int color);
void DrawLine(cdc::Vector3* v0, cdc::Vector3* v1, int color);
void DrawBoundingBox(cdc::Vector3* v0, cdc::Vector3* v1, int color);
void DrawBoundingBox(cdc::Vector3* v0, cdc::Vector3* v1, cdc::Vector3* v2, cdc::Vector3* v3, int color);

void DrawBox(cdc::Vector3* v0, cdc::Vector3* v1, cdc::Vector3* v2, cdc::Vector3* v3, int color);