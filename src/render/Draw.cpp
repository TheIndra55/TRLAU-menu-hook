#include "Draw.h"
#include "util/Hooking.h"

void TRANS_TransToDrawVertexV4f(DRAWVERTEX* v, cdc::Vector3* vec)
{
	auto addr = GET_ADDRESS(0x402EF0, 0x402F20, 0x49F990);

	Hooking::Call(addr, v, vec);
}

void TRANS_RotTransPersVectorf(cdc::Vector3* srcvector, cdc::Vector3* dstvector)
{
	auto addr = GET_ADDRESS(0x402B20, 0x402B50, 0x49F630);

	Hooking::Call(addr, srcvector, dstvector);
}

void TRANS_TransToDrawVertex(cdc::Vector3* vec, DRAWVERTEX* v)
{
	auto addr = GET_ADDRESS(0x000000, 0x000000, 0x48A450);

	Hooking::Call(addr, vec, v);
}

void DRAW_DrawQuads(int flags, int tpage, DRAWVERTEX* verts, int numquads)
{
	auto addr = GET_ADDRESS(0x406720, 0x406D70, 0x5BFB20);

	Hooking::Call(addr, flags, tpage, verts, numquads);
}

void DRAW_DrawLines(LINEVERTEX* verts, int numlines)
{
	auto addr = GET_ADDRESS(0x000000, 0x000000, 0x5BFCD0);

	Hooking::Call(addr, verts, numlines);
}

void DRAW_DrawTriangles(int flags, int tpage, DRAWVERTEX* verts, int numtris)
{
	auto addr = GET_ADDRESS(0x406A40, 0x407570, 0x5BFA40);

	Hooking::Call(addr, flags, tpage, verts, numtris);
}

inline void TransformToVertex(DRAWVERTEX* v, cdc::Vector3* vec)
{
#ifndef TR8
	TRANS_TransToDrawVertexV4f(v, vec);
#else
	TRANS_TransToDrawVertex(vec, v);
#endif
}

void DrawTriangle(cdc::Vector3* v0, cdc::Vector3* v1, cdc::Vector3* v2, int color)
{
	DRAWVERTEX verts[3];

	TransformToVertex(verts, v0);
	TransformToVertex(&verts[1], v1);
	TransformToVertex(&verts[2], v2);

	verts[0].color = color;
	verts[1].color = color;
	verts[2].color = color;

	DRAW_DrawTriangles(2, 0, verts, 1);
}

void DrawPlane(cdc::Vector3* v0, cdc::Vector3* v1, int color)
{
	DRAWVERTEX verts[6];

	auto v2 = cdc::Vector3{ v0->x, v0->y, v1->z };
	auto v3 = cdc::Vector3{ v1->x, v1->y, v0->z };

	TransformToVertex(verts, v0);
	TransformToVertex(&verts[1], &v2);
	TransformToVertex(&verts[2], v1);
	TransformToVertex(&verts[3], &v3);
	TransformToVertex(&verts[4], v1);
	TransformToVertex(&verts[5], v0);

	verts[0].color = color;
	verts[1].color = color;
	verts[2].color = color;
	verts[3].color = color;
	verts[4].color = color;
	verts[5].color = color;

	DRAW_DrawTriangles(2, 0, verts, 2);
}

// Scuffed ass line, TODO fix
void DrawLine(cdc::Vector3* v0, cdc::Vector3* v1, int color)
{
	DRAWVERTEX verts[6];

	auto v2 = *v1;
	auto v3 = *v0;

	v2.z += 20.f;
	v3.z += 20.f;
	v2.y += 20.f;
	v3.y += 20.f;

	TransformToVertex(verts, v0);
	TransformToVertex(&verts[1], &v2);
	TransformToVertex(&verts[2], v1);
	TransformToVertex(&verts[3], &v3);
	TransformToVertex(&verts[4], v1);
	TransformToVertex(&verts[5], v0);

	verts[0].color = color;
	verts[1].color = color;
	verts[2].color = color;
	verts[3].color = color;
	verts[4].color = color;
	verts[5].color = color;

	DRAW_DrawTriangles(2, 0, verts, 2);
}

void DrawBoundingBox(cdc::Vector3* v0, cdc::Vector3* v1, int color)
{
	auto a1 = cdc::Vector3{ v0->x, v0->y, v0->z };
	auto a2 = cdc::Vector3{ v1->x, v0->y, v0->z };
	auto a3 = cdc::Vector3{ v1->x, v1->y, v0->z };
	auto a4 = cdc::Vector3{ v0->x, v1->y, v0->z };

	auto b1 = cdc::Vector3{ v0->x, v0->y, v1->z };
	auto b2 = cdc::Vector3{ v1->x, v0->y, v1->z };
	auto b3 = cdc::Vector3{ v1->x, v1->y, v1->z };
	auto b4 = cdc::Vector3{ v0->x, v1->y, v1->z };

	DrawLine(&a1, &a2, color);
	DrawLine(&a2, &a3, color);
	DrawLine(&a3, &a4, color);
	DrawLine(&a4, &a1, color);

	DrawLine(&b1, &b2, color);
	DrawLine(&b2, &b3, color);
	DrawLine(&b3, &b4, color);
	DrawLine(&b4, &b1, color);

	DrawLine(&b1, &a1, color);
	DrawLine(&b2, &a2, color);
	DrawLine(&b3, &a3, color);
	DrawLine(&b4, &a4, color);
}

void DrawBoundingBox(cdc::Vector3* v0, cdc::Vector3* v1, cdc::Vector3* v2, cdc::Vector3* v3, int color)
{
	// Yes this is terrible
	auto a1 = cdc::Vector3{ v0->x, v0->y, v0->z };
	auto a2 = cdc::Vector3{ v2->x, v2->y, v0->z };
	auto a3 = cdc::Vector3{ v1->x, v1->y, v0->z };
	auto a4 = cdc::Vector3{ v3->x, v3->y, v0->z };

	auto b1 = cdc::Vector3{ v0->x, v0->y, v1->z };
	auto b2 = cdc::Vector3{ v2->x, v2->y, v1->z };
	auto b3 = cdc::Vector3{ v1->x, v1->y, v1->z };
	auto b4 = cdc::Vector3{ v3->x, v3->y, v1->z };

	DrawLine(&a1, &a2, color);
	DrawLine(&a2, &a3, color);
	DrawLine(&a3, &a4, color);
	DrawLine(&a4, &a1, color);

	DrawLine(&b1, &b2, color);
	DrawLine(&b2, &b3, color);
	DrawLine(&b3, &b4, color);
	DrawLine(&b4, &b1, color);

	DrawLine(&b1, &a1, color);
	DrawLine(&b2, &a2, color);
	DrawLine(&b3, &a3, color);
	DrawLine(&b4, &a4, color);
}

void DrawBox(cdc::Vector3* v0, cdc::Vector3* v1, cdc::Vector3* v2, cdc::Vector3* v3, int color)
{
	auto a1 = cdc::Vector3{ v0->x, v0->y, v0->z };
	auto a2 = cdc::Vector3{ v2->x, v2->y, v0->z };
	auto a3 = cdc::Vector3{ v1->x, v1->y, v0->z };
	auto a4 = cdc::Vector3{ v3->x, v3->y, v0->z };

	auto b1 = cdc::Vector3{ v0->x, v0->y, v1->z };
	auto b2 = cdc::Vector3{ v2->x, v2->y, v1->z };
	auto b3 = cdc::Vector3{ v1->x, v1->y, v1->z };
	auto b4 = cdc::Vector3{ v3->x, v3->y, v1->z };

	DrawPlane(&a1, &b2, color);
	DrawPlane(&a2, &b3, color);
	DrawPlane(&a3, &b4, color);
	DrawPlane(&a4, &b1, color);

	DrawPlane(&a1, &a3, color);
	DrawPlane(&b1, &b3, color);
}