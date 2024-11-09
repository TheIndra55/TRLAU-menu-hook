#pragma once

struct NtUnlockableCostume
{
	int costumeIndex;
};

class NtUnlockableCostume_ScriptType
{
public:
	static void SwitchPlayerCostume(NtUnlockableCostume* costume);
};