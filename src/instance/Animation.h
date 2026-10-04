#pragma once

#include "cdc/math/Vector.h"

struct Instance;
class AnimProcessor;

#ifndef TR8
enum G2AnimCallbackMsg : __int32
{
	G2ANIM_MSG_DONE = 0x1,
	G2ANIM_MSG_LOOPPOINT = 0x2,
	G2ANIM_MSG_SECTION_INTERPDONE = 0x3,
	G2ANIM_MSG_SEGCTRLR_INTERPDONE = 0x4,
	G2ANIM_MSG_SWALARMSET = 0x5,
	G2ANIM_MSG_PLAYEFFECT = 0x6,
	G2ANIM_MSG_PLAYEFFECT_HITREACTION = 0x7,
	G2ANIM_MSG_ANIM_QUEUE_EMPTY = 0x8,
	G2ANIM_MSG_SWITCH_KEYLIST = 0x9,
};

enum G2ANIM_FLAGS
{
	G2ANIM_MAXSEGMENTS = 0xFF,
	G2ANIM_ALARMFLAG_STOPPED_END = 0x1,
	G2ANIM_ALARMFLAG_STOPPED_BEGIN = 0x2,
	G2ANIM_ALARMFLAG_LOOPPOINT = 0x4,
	G2ANIM_ALARMFLAG_TRIGGERED = 0x8,
	G2ANIM_ALARMFLAG_INTERPDONE = 0x10,
	G2ANIM_ALARMFLAG_SWALARM = 0x20,
	G2ANIM_ALARMFLAG_DISABLE_SOUND = 0x40,
	G2ANIM_ALARMFLAG_DISABLE_EFFECT = 0x80,
	G2ANIM_ALARMFLAG_PLAYED = 0x100,
	G2ANIM_ALARMFLAG_STOPPED_EVENT = 0x200,
	G2ANIM_ALARMFLAG_STOPPED = 0x3,
	G2ANIM_SECTIONFLAG_PAUSED = 0x1,
	G2ANIM_SECTIONFLAG_LOOPING = 0x2,
	G2ANIM_SECTIONFLAG_REWINDING = 0x4,
	G2ANIM_SECTIONFLAG_NOROOTTRANS = 0x8,
	G2ANIM_SECTIONFLAG_CTRLRDIRTY = 0x10,
	G2ANIM_SECTIONFLAG_HOSTANIM = 0x20,
	G2ANIM_SECTIONFLAG_ANIMHASROOTMOTION = 0x40,
	G2ANIM_SECTIONFLAG_UPDATED = 0x80,
	G2ANIM_SECTIONFLAG_NOROOTROT = 0x100,
	G2ANIM_SECTIONFLAG_INTERPINFO = 0x200,
	G2ANIM_FLAG_NEWTIME = 0x1,
	G2ANIM_FLAG_NEWTIME_DEFERRED = 0x2,
	G2ANIM_FLAG_UPDATE_STORED_FRAME_CACHE = 0x4,
	G2ANIM_FLAG_APPLY_CONTROLLERS_TO_STORED_FRAME_CACHE = 0x8,
	G2ANIM_FLAG_MIRROR_CURRENT = 0x10,
	G2ANIM_FLAG_MIRROR_NEXT = 0x20,
	G2ANIM_FLAG_ROOT_TRANS_PRESENT = 0x40,
	G2ANIM_FLAG_ROOT_ROT_PRESENT = 0x80,
	G2ANIM_ROTCHANNEL_ACTIVE = 0x1,
	G2ANIM_SCALECHANNEL_ACTIVE = 0x2,
	G2ANIM_TRANSCHANNEL_ACTIVE = 0x4,
	G2ANIM_CHANNELMASK_XROT = 0x1,
	G2ANIM_CHANNELMASK_YROT = 0x2,
	G2ANIM_CHANNELMASK_ZROT = 0x4,
	G2ANIM_CHANNELMASK_ROT = 0x7,
	G2ANIM_CHANNELMASK_XSCALE = 0x10,
	G2ANIM_CHANNELMASK_YSCALE = 0x20,
	G2ANIM_CHANNELMASK_ZSCALE = 0x40,
	G2ANIM_CHANNELMASK_SCALE = 0x70,
	G2ANIM_CHANNELMASK_XTRANS = 0x100,
	G2ANIM_CHANNELMASK_YTRANS = 0x200,
	G2ANIM_CHANNELMASK_ZTRANS = 0x400,
	G2ANIM_CHANNELMASK_TRANS = 0x700,
	G2ANIM_CTRLRFLAG_APPLYPARENTROT = 0x1,
	G2ANIM_CTRLRFLAG_APPLYPARENTSCALE = 0x2,
	G2ANIM_CTRLRFLAG_APPLYPARENTTRANS = 0x4,
	G2ANIM_CTRLRFLAG_APPLYNOTHING = 0x8,
	G2ANIM_CTRLRBIT_FN = 0x80,
	G2ANIM_CTRLRBIT_LOCAL = 0x2,
	G2ANIM_CTRLRBIT_ADD = 0x4,
	G2ANIM_CTRLRBIT_ROT = 0x8,
	G2ANIM_CTRLRBIT_SCALE = 0x10,
	G2ANIM_CTRLRBIT_TRANS = 0x20,
	G2ANIM_CTRLRBIT_LAST = 0x40,
	G2ANIM_CTRLRBIT_SETWORLD = 0x0,
	G2ANIM_CTRLRBIT_SETLOCAL = 0x2,
	G2ANIM_CTRLRBIT_ADDLOCAL = 0x6,
	G2ANIM_CTRLRBIT_ADDWORLD = 0x44,
	G2ANIM_CTRLRBITMASK_FRAME = 0x2,
	G2ANIM_CTRLRBITMASK_TYPE = 0x46,
	G2ANIM_CTRLRBITMASK_CHANNEL = 0x38,
};

enum RootInterpMode : __int32
{
	kRootInterpMode_ConserveMotion = 0x0,
	kRootInterpMode_MorphSpeed = 0x1,
	kRootInterpMode_MorphVelocity = 0x2,
	kRootInterpMode_UserTransform = 0x3,
};

struct AnimFragment
{
	float mTransX;
	float mTransY;
	float mTransZ;
	float mRotX;
	float mRotY;
	float mRotZ;
	__int16 mAnimID;
	__int16 mKeyCount;
	__int16 mTimePerKey;
	unsigned __int8 mSegmentCount;
	unsigned __int8 mSectionCount;
	int mSectionDataOffset[1];
};
#endif

struct AnimFxHeader
{
#ifndef TR8
	unsigned __int16 info;
	__int16 keyframeID;
	unsigned __int8 fxID;
	unsigned __int8 pad[3];
#endif
};

class AnimFragmentProcessor
{
public:
#ifndef TR8
	cdc::Quat mRootRot;
	cdc::Vector3 mRootTrans;
	AnimProcessor* mAnimProcessor;
	AnimFragment* mKeylist;
	float mElapsedTime;
	float mStoredTime;
	void* mFxListHost;
	AnimFxHeader* mFxList;
	float* mSwAlarmTable;
	float mLoopStartTime;
	float mLoopEndTime;
	int(__cdecl * mCallback)(AnimProcessor*, int, G2AnimCallbackMsg, int, int, void*);
	void* mCallbackData;
	G2ANIM_FLAGS mAlarmFlags;
	float mSpeedAdjustment;
	float mAccumulatedTime;
	unsigned __int16 mFlags;
	unsigned __int16 mKeylistID;
	unsigned __int8 mSectionID;
	unsigned __int8 mFirstSeg;
	unsigned __int8 mSegCount;
	char gap5F;
	RootInterpMode mRootInterpMode;
	RootInterpMode mNewRootInterpMode;
	unsigned __int8 pad[3];
#endif
};


class AnimProcessor
{
public:
#ifndef TR8
	char pad1[288];
	AnimFragmentProcessor* mSection;
	//char pad1[336];
	char pad2[44];
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