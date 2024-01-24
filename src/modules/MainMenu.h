#pragma once

#include "Module.h"

class MainMenu : public Module
{
private:
	void BirthObject(char* name);

public:
	void OnDraw();
	void OnFrame();
};