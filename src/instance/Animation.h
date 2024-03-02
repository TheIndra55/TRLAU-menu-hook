#pragma once

struct Instance;

class AnimProcessor
{
public:
#ifndef TR8
	char pad1[336];
#else
	char pad1[246];
#endif

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
#ifndef TR8
	BlendProcessor mBlendProcessor;
#else
	char pad1[20];
#endif
	AnimProcessor* mAnimProcessor;
};

#ifndef TR8
struct AnimListEntry
{
	__int16 animationID;
	__int16 animationNumber;
};
#else
struct AnimListEntry
{
	__int16 animationID;
	__int16 animationNumber;

	void* unk1;
	char* debugName;
};
#endif

void G2EmulationInstanceSetAnimation(Instance* instance, int CurrentSection, int NewAnim, int NewFrame, int Frames);
void G2EmulationInstanceSetMode(Instance* instance, int CurrentSection, int Mode);
int G2EmulationInstanceQueryAnimation(Instance* instance, int CurrentSection);