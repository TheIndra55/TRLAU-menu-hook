#pragma once

#include "Module.h"

class Frontend : public Module
{
private:
	bool m_hideHud = false;

public:
	void OnMenu();
};