#pragma once

#include <xmmintrin.h>

namespace cdc
{
	class Vector
	{
	public:
		union
		{
			__m128 vec128;

			struct
			{
				float x;
				float y;
				float z;
				float w;
			};
		};

		void operator +=(Vector* b);
		void operator -=(Vector* b);
		void operator *=(Vector* b);
		void operator /=(Vector* b);

		void operator *=(float b);
		void operator /=(float b);

		float operator -(cdc::Vector* b);
	};

	class Vector2 : public Vector
	{
	};

	class Vector3 : public Vector
	{
	};

	class Vector4 : public Vector
	{
	};

	class Euler : public Vector
	{
	};

	class Quat : public Vector
	{
	};
}