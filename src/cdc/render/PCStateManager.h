#pragma once

#include <d3d9.h>

#include "PCInternalResource.h"

namespace cdc
{
	class PCStateManager : public PCInternalResource
	{
	public:
		IDirect3DDevice9* m_pD3DDevice;

		void SetRenderState(D3DRENDERSTATETYPE state, unsigned int value);
	};
}