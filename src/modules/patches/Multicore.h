#pragma once

namespace cdc
{
	class JobChainImplWithThreads
	{
	public:
		static bool(*s_StartSystem)(int);

		static bool StartSystem(int numThreads);
	};
}