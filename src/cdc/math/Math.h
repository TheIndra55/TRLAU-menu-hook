#pragma once

#include "Vector.h"
#include "Matrix.h"

namespace cdc
{
	Vector3 Mul3x3(Matrix* matA, Vector3* vecB);
	Vector3 Mul3x4(Matrix* matA, Vector3* vecB);
}