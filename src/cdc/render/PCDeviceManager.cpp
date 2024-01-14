#include "PCDeviceManager.h"

cdc::PCDeviceManager* cdc::PCDeviceManager::s_pInstance = nullptr;

bool cdc::PCDeviceManager::IsStatusOk()
{
	return m_status == kStatusOk;
}

bool cdc::PCDeviceManager::IsCreatingResources()
{
	return m_bIsRecreatingResources;
}

IDirect3DDevice9* cdc::PCDeviceManager::GetD3DDevice()
{
	return m_pD3DDevice;
}

HWND cdc::PCDeviceManager::GetWindow()
{
	return m_hFocusWindow;
}

void cdc::PCDeviceManager::AddDeviceResource(PCInternalResource* resource)
{
	auto func = reinterpret_cast<void(__thiscall*)(PCDeviceManager*, PCInternalResource*)>(0x00616F10);

	func(this, resource);
}