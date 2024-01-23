#include "Vector.h"

void cdc::Vector::operator +=(Vector* b)
{
	vec128 = _mm_add_ps(vec128, b->vec128);
}

void cdc::Vector::operator -=(Vector* b)
{
	vec128 = _mm_sub_ps(vec128, b->vec128);
}

void cdc::Vector::operator *=(Vector* b)
{
	vec128 = _mm_mul_ps(vec128, b->vec128);
}

void cdc::Vector::operator /=(Vector* b)
{
	vec128 = _mm_div_ps(vec128, b->vec128);
}

void cdc::Vector::operator *=(float b)
{
	vec128 = _mm_mul_ps(vec128, _mm_shuffle_ps(_mm_set_ss(b), _mm_set_ss(b), _MM_SHUFFLE(0, 0, 0, 0)));
}

void cdc::Vector::operator /=(float b)
{
	vec128 = _mm_div_ps(vec128, _mm_shuffle_ps(_mm_set_ss(b), _mm_set_ss(b), _MM_SHUFFLE(0, 0, 0, 0)));
}