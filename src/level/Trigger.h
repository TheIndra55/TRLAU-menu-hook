#pragma once

#include "instance/Instance.h"

enum TriggerShape
{
	TriggerShape_Box = 1,
	TriggerShape_Sphere = 2
};

struct IntroDataTrigger : IntroData
{
	char pad1[44];

	TriggerShape shape;
	float LocalXExtent;
	float LocalYExtent;
	float LocalZExtent;
	float Radius;
};

struct NsInstance
{
	char pad1[32];

	Instance* m_instance;
};

struct NsTriggerPlaneBase : NsInstance
{
	unsigned int m_flags;
};

struct NsTriggerVolumeBase : NsInstance
{
	unsigned int m_flags;
};