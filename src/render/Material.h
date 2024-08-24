#pragma once

#include "cdc/render/CommonMaterial.h"

struct MaterialEntry
{
	cdc::CommonMaterial* pMaterial;
	int field_4;
	unsigned int refCount;
	unsigned int size;
	unsigned int versionID;
};