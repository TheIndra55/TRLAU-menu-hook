#include "PCInternalResource.h"
#include "PCDeviceManager.h"

cdc::PCInternalResource::PCInternalResource() : m_pPrev(nullptr), m_pNext(nullptr)
{
	PCDeviceManager::s_pInstance->AddDeviceResource(this);
}

cdc::PCInternalResource::~PCInternalResource()
{
}

void cdc::PCInternalResource::OnConstruct()
{
	auto deviceManager = cdc::PCDeviceManager::s_pInstance;

	if (deviceManager->IsStatusOk() && !deviceManager->IsCreatingResources())
	{
		OnCreateDevice();
	}
}