#include "Multicore.h"

#include "Hook.h"
#include "modules/Log.h"

bool(*cdc::JobChainImplWithThreads::s_StartSystem)(int);

bool cdc::JobChainImplWithThreads::StartSystem(int numThreads)
{
	if (numThreads > 32)
	{
		numThreads = 32;
	}

	Hook::GetInstance().GetModule<Log>()->WriteLine("cdc::JobChainImplWithThreads::StartSystem(%d)", numThreads);

	return s_StartSystem(numThreads);
}