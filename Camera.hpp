#pragma once
#include "Game.hpp"

void InstallCameraHooks();

struct Camera
{
	cdc::Vector position;
	char padding[608];
	cdc::Vector rotation;
};