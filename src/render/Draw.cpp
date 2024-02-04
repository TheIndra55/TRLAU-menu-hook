#include "Draw.h"
#include "util/Hooking.h"

void TRANS_TransToDrawVertexV4f(DRAWVERTEX* v, cdc::Vector3* vec)
{
	Hooking::Call(0x402F20, v, vec);
}

void TRANS_RotTransPersVectorf(cdc::Vector3* srcvector, cdc::Vector3* dstvector)
{
	Hooking::Call(0x402B50, srcvector, dstvector);
}

void DRAW_DrawQuads(int flags, int tpage, DRAWVERTEX* verts, int numquads)
{
	Hooking::Call(0x406D70, flags, tpage, verts, numquads);
}

void DRAW_DrawTriangles(int flags, int tpage, DRAWVERTEX* verts, int numtris)
{
	Hooking::Call(0x407570, flags, tpage, verts, numtris);
}

void DrawPlane(cdc::Vector3* v0, cdc::Vector3* v1, int color)
{
	DRAWVERTEX verts[6];

	auto v2 = cdc::Vector3{ v0->x, v0->y, v1->z };
	auto v3 = cdc::Vector3{ v1->x, v1->y, v0->z };

	TRANS_TransToDrawVertexV4f(verts, v0);
	TRANS_TransToDrawVertexV4f(&verts[1], &v2);
	TRANS_TransToDrawVertexV4f(&verts[2], v1);
	TRANS_TransToDrawVertexV4f(&verts[3], &v3);
	TRANS_TransToDrawVertexV4f(&verts[4], v1);
	TRANS_TransToDrawVertexV4f(&verts[5], v0);

	verts[0].color = color;
	verts[1].color = color;
	verts[2].color = color;
	verts[3].color = color;
	verts[4].color = color;
	verts[5].color = color;

	DRAW_DrawTriangles(2, 0, verts, 2);
}