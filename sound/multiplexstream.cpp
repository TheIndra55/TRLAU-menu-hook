#include <stdio.h>
#include "../include/MinHook.h"
#include "multiplexstream.hpp"
#include "../Hooking.hpp"

void* (__cdecl* SNDAPP_StreamOpenFile)(const char* filename);
int (__cdecl* SNDAPP_StreamGetSize)(void* file);

void(__cdecl* SNDAPP_StreamRequestData)(void* file, int offset, int size, void* callback, void* context);

void memset32(void* dst, int val, int count);

extern int(__thiscall* MSFileSystem_FileExists)(int _this, const char* file);

bool __fastcall MultiplexStreamImpl_Init(MultiplexStreamImpl* _this, int _, StreamType type, const char* name)
{
	int assignment;
	char filename[_MAX_PATH];

	if (type == kCinematicStream)
	{
		_this->m_streamBufSize = *(int*)0x66323C;
		sprintf(filename, "%s%s.mul", (char*)0x754778, name);

		assignment = globalSoundInfo->soundGroupAssignments->cine;
	}
	else if (type == kMusicStream)
	{
		_this->m_streamBufSize = *(int*)0x663240;
		sprintf(filename, "%s%s.mul", (char*)0x86330C, name);

		assignment = globalSoundInfo->soundGroupAssignments->music;
	}
	else if (type == kSoundStream)
	{
		_this->m_streamBufSize = *(int*)0x66323C;
		sprintf(filename, "%s%s.mul", (char*)0x754678, name);

		assignment = globalSoundInfo->soundGroupAssignments->sfx;
	}
	else
	{
		return 0;
	}

	int len = 12;
	if (_this->m_streamState == kPlaying)
		len = _this->m_channelCount;

	if (len > 0)
	{
		memset32(_this->m_group, assignment, len);
	}

#if 1
	// concatenate f:\trae\ and check if file exists on disk
	char msfilename[_MAX_PATH];
	sprintf_s(msfilename, "\\" CONFIGNAME "\\%s", filename);

	if (MSFileSystem_FileExists(*(int*)DISKFS, msfilename))
	{
		Hooking::GetInstance().GetMenu()->Log("%s exists, loading that one instead\n", msfilename);

		strcpy_s(filename, msfilename);
		isDiskFS = true;
	}
#endif

	auto handle = SNDAPP_StreamOpenFile(filename);

	isDiskFS = false;
	_this->m_appHandle = handle;

	if (!handle)
	{
		Hooking::GetInstance().GetMenu()->Log("Failed to open sound %s with type %d\n", filename, type);
		return 0;
	}

	auto size = SNDAPP_StreamGetSize(_this->m_appHandle);

	_this->m_streamSize = size;
	_this->m_streamType = type;
	_this->m_fsoundStreamBufSize = *(int*)0x663244;
	_this->m_streamState = kReadingHeader;
	_this->m_loadingBusy = true;
	SNDAPP_StreamRequestData(handle, 0, 2048, (void*)0x00419690, _this);
	_this->m_isCueing = true;
	_this->m_createdInPauseMode = (*(bool*)0x8B76C0) != 0;

	return 1;
}

// https://stackoverflow.com/a/15210350/9398242
void memset32(void* dst, int val, int count)
{
	for (int i = 0; i < count; i++)
	{
		((int*)dst)[i] = val;
	}
}

void InstallSoundHooks()
{
	// no trampoline since we rewrote the entire function
	MH_CreateHook((void*)0x004198D0, MultiplexStreamImpl_Init, nullptr);

	SNDAPP_StreamOpenFile = reinterpret_cast<void* (__cdecl*)(const char*)>(0x005CEFC0);
	SNDAPP_StreamGetSize = reinterpret_cast<int(__cdecl*)(void*)>(0x005CF0A0);
	SNDAPP_StreamRequestData = reinterpret_cast<void(__cdecl*)(void*, int, int, void*, void*)>(0x005CF1A0);
}