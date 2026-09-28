#include "Animation.h"
#include "util/Hooking.h"
// #include "Hook.h"
// #include "modules/Patches.h"
// #include "modules/Log.h"

//static Patches* s_patches;
//static Log* s_log;

void G2EmulationInstanceSetAnimation(Instance* instance, int CurrentSection, int NewAnim, int NewFrame, int Frames)
{
	auto addr = GET_ADDRESS(0x4DEC30, 0x4DE690, 0x5B1EA0);

	Hooking::Call(addr, instance, CurrentSection, NewAnim, NewFrame, Frames);
}

void G2EmulationInstanceSetMode(Instance* instance, int CurrentSection, int Mode)
{
	auto addr = GET_ADDRESS(0x4DED90, 0x4DE7F0, 0x5B1F50);

	Hooking::Call(addr, instance, CurrentSection, Mode);
}

int G2EmulationInstanceQueryAnimation(Instance* instance, int CurrentSection)
{
	auto addr = GET_ADDRESS(0x4DEE50, 0x4DE8B0, 0x5B1FC0);

	return Hooking::CallReturn<int>(addr, instance, CurrentSection);
}

#ifdef TRAE
//void(__fastcall* AnimProcessor::s_SwapBones)(AnimProcessor* pthis);
//
//void __fastcall AnimProcessor::SwapBones(AnimProcessor* pthis)
//{
//	s_patches = Hook::GetInstance().GetModule<Patches>().get();
//	s_log = Hook::GetInstance().GetModule<Log>().get();
//
//	//if (patches->IsNoAnimMirror())
//	if (s_patches->IsNoAnimMirror())
//	{
//		s_log->WriteLine("AnimProcessor::SwapBones() with animID 0x%X", pthis->mSection->mKeylist->mAnimID);
//
//		switch (pthis->mSection->mKeylist->mAnimID)
//		{
//		case 0x51: //GRAPPLE_QUICKGRAPPLE
//		case 0x16A: //ROPE_CLIMBDOWN
//		case 0x171: //ROPE_ENDPUMP
//		case 0x172: //ROPE_IDLEHANG
//		case 0x179: //ROPE_STARTPUMP
//		case 0x17F: //ROPE_TURNLEFT
//		case 0x180: //ROPE_TURNRIGHT
//		case 0x1BC: //WALLGRAPPLE_CLIMBUP
//			s_log->WriteLine("	Skipping SwapBones() for animID 0x%X", pthis->mSection->mKeylist->mAnimID);
//		//default:
//			return;
//		}
//	}
//
//	s_SwapBones(pthis);
//}
#endif