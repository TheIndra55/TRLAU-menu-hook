#pragma once

#include "cdc/render/PCInternalResource.h"

class Menu : public cdc::PCInternalResource
{
private:
	bool m_initialized = false;

public:
	Menu();

	void OnPresent();

	bool OnCreateDevice();
	void OnDestroyDevice();

private:
	void Draw();
};