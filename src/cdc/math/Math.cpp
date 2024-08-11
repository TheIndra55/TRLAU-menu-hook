#include "Math.h"

cdc::Vector3 cdc::Mul3x3(Matrix* matA, Vector3* vecB)
{
	cdc::Vector3 result;

	result.vec128 = _mm_add_ps(
		_mm_mul_ps(matA->col0.vec128, _mm_shuffle_ps(vecB->vec128, vecB->vec128, _MM_SHUFFLE(0, 0, 0, 0))),
		_mm_add_ps(
			_mm_mul_ps(matA->col1.vec128, _mm_shuffle_ps(vecB->vec128, vecB->vec128, _MM_SHUFFLE(1, 1, 1, 1))),
			_mm_mul_ps(matA->col2.vec128, _mm_shuffle_ps(vecB->vec128, vecB->vec128, _MM_SHUFFLE(2, 2, 2, 2)))));
	
	return result;
}

cdc::Vector3 cdc::Mul3x4(Matrix* matA, Vector3* vecB)
{
	cdc::Vector3 result;

	result.vec128 = _mm_add_ps(
		_mm_mul_ps(matA->col0.vec128, _mm_shuffle_ps(vecB->vec128, vecB->vec128, _MM_SHUFFLE(0, 0, 0, 0))),
		_mm_add_ps(
			_mm_mul_ps(matA->col1.vec128, _mm_shuffle_ps(vecB->vec128, vecB->vec128, _MM_SHUFFLE(1, 1, 1, 1))),
			_mm_add_ps(
				_mm_mul_ps(matA->col2.vec128, _mm_shuffle_ps(vecB->vec128, vecB->vec128, _MM_SHUFFLE(2, 2, 2, 2))),
				matA->col3.vec128)));
	
	return result;
}