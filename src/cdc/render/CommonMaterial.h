#pragma once

#include "MaterialData.h"

namespace cdc
{
	class IMaterial
	{
	public:
		virtual ~IMaterial() = 0;
	};

	class CommonMaterial : public IMaterial
	{
	private:
		MaterialData* m_pData;

	public:
		MaterialData* GetMaterialData() const noexcept;
	};
}