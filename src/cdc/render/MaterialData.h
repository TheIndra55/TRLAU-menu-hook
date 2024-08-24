#pragma once

namespace cdc
{
	struct MaterialData
	{
		char pad[88];

		__int16 numParameters;
		float* parameters;
	};
}