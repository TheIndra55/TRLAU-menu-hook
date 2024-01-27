#pragma once

struct Instance;

class AnimProcessor
{
public:
	char pad1[336];

	unsigned __int8 mSectionCount;
	unsigned __int8 mSectionsAllocated;
};

class AnimComponent;

class BlendProcessor
{
public:
	AnimComponent* mAnimComponent;
};

class AnimComponent
{
public:
	BlendProcessor mBlendProcessor;
	AnimProcessor* mAnimProcessor;
};

struct AnimListEntry
{
	__int16 animationID;
	__int16 animationNumber;
};

void G2EmulationInstanceSetAnimation(Instance* instance, int CurrentSection, int NewAnim, int NewFrame, int Frames);
void G2EmulationInstanceSetMode(Instance* instance, int CurrentSection, int Mode);
int G2EmulationInstanceQueryAnimation(Instance* instance, int CurrentSection);