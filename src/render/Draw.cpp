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