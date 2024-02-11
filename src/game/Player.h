#pragma once

struct TuneData;

struct GeneralAttributes
{
	float height;
	float crouchHeight;
	float ankleUpperOffset;
	float ankleLowerOffset;
	float ankleLength;
	float mass;
	float leftFootLockHipLimitMin;
	float leftFootLockHipLimitMax;
	float rightFootLockHipLimitMin;
	float rightFootLockHipLimitMax;
};

struct OldRazielData
{
	char pad1[116];

	float HealthInitial;
	float LowHealth;
};

struct PlayerProp
{
	unsigned int playerVersion;

	TuneData* tuneData;
	GeneralAttributes general;
	OldRazielData oldData;
};

void IncrHealth(float amount);