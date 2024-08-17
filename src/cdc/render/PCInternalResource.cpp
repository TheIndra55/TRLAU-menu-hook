#include "PCInternalResource.h"
#include "PCDeviceManager.h"

cdc::PCInternalResource::PCInternalResource() : m_pPrev(nullptr), m_pNext(nullptr)
{
	PCDeviceManager::GetInstance()->AddDeviceResource(this);
}

cdc::PCInternalResource::~PCInternalResource()
{
}

#if TR8
bool cdc::PCInternalResource::OnPostResetDevice()
{
	return true;
}

void cdc::PCInternalResource::OnPreResetDevice()
{
}

void cdc::PCInternalResource::OnSettingsChanged()
{
}
#endif

void cdc::PCInternalResource::OnConstruct()
{
	auto deviceManager = PCDeviceManager::GetInstance();

	if (deviceManager->IsStatusOk() && !deviceManager->IsCreatingResources())
	{
		OnCreateDevice();
	}
}