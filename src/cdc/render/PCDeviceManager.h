#pragma once

#include <Windows.h>
#include <d3d9.h>

#include "PCInternalResource.h"

class Hook;

namespace cdc
{
	class PCDeviceManager
	{
	public:
		enum Status
		{
			kStatusOk
		};

	private:
		char pad1[16];

		Status m_status;
		HWND m_hFocusWindow;

		char pad2[8];

		IDirect3DDevice9* m_pD3DDevice;
		D3DCAPS9 m_d3dCaps;

		bool m_bIsRecreatingResources;

		static PCDeviceManager* s_pInstance;

	public:
		bool IsStatusOk();
		bool IsCreatingResources();

		IDirect3DDevice9* GetD3DDevice();
		HWND GetWindow();

		void AddDeviceResource(PCInternalResource* resource);

		static PCDeviceManager* GetInstance();

		friend class Hook;
	};
}