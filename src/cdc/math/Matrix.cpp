#include "Matrix.h"
#include "util/Hooking.h"

void cdc::Matrix::Build(Euler* euler)
{
	// Too lazy to implement myself
	auto addr = GET_ADDRESS(0x000000, 0x000000, 0x49DD70);

	Hooking::ThisCall(addr, this, euler);
}