#pragma once

#include "Vector.h"

namespace cdc
{
	class Matrix
	{
	public:
		Vector col0;
		Vector col1;
		Vector col2;
		Vector col3;

		void Build(Euler* euler);
		void Build_XYZOrder(cdc::Euler* euler);
	};
}