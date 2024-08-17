#include <Hooking.Patterns.h>

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

cdc::PCDeviceManager* cdc::PCDeviceManager::GetInstance()
{
	return s_pInstance;
}

void cdc::PCDeviceManager::AddDeviceResource(PCInternalResource* resource)
{
#ifndef TR8
	auto match = hook::pattern("8B 41 08 33 D2 3B C2 8B 44 24 04").count(1);
#else
	auto match = hook::pattern("8B 44 24 04 33 D2 39 51 08 89 50").count(1);
#endif

	auto func = (void(__thiscall*)(PCDeviceManager*, PCInternalResource*))match.get_first();
	func(this, resource);
}