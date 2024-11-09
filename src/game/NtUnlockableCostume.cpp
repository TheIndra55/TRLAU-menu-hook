#include "NtUnlockableCostume.h"
#include "util/Hooking.h"

void NtUnlockableCostume_ScriptType::SwitchPlayerCostume(NtUnlockableCostume* costume)
{
	Hooking::Call(0x79DB50, costume);
}