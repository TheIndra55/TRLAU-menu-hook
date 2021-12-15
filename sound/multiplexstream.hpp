#pragma once

#include "../Game.hpp"
#include "../util.hpp"

#define globalSoundInfo VAR_U_(0x008A687C, GlobalSoundInfo*)

enum StreamType
{
	kSoundStream = 0x0,
	kCinematicStream = 0x1,
	kMusicStream = 0x2,
};

enum StreamState
{
	kReadingHeader = 0x1,
	kPrebuffering = 0x2,
	kReadyToPlay = 0x3,
	kPlaying = 0x4,
	kFinished = 0x5,
	kCancelled = 0x6,
	kDestroyed = 0x7,
};

struct __declspec(align(16)) MultiplexStreamImpl
{
	char skip[8]; // CinematicHandlerHost bullshit MEH
	MultiplexStreamImpl* m_prev;
	MultiplexStreamImpl* m_next;
	void* m_cinematicStream;
	void* m_cinematicHandler;
	int m_channelCount;
	void* m_streamBuf[12];
	int m_channel[12];
	void* m_fsoundStream[12];
	StreamState m_streamState;
	int m_streamBufSize;
	int m_fsoundStreamBufSize;
	int m_processState;
	int m_packetBytesRemaining;
	int m_readingDataOffset;
	int m_channelCurrentlyUpdating;
	int m_currentChannelOffset;
	int m_currentPacketSize;
	char m_strname[128];
	void* m_appHandle;
	StreamType m_streamType;
	int m_streamSize;
	char m_streamHeader[0xCC];
	int m_currentStreamPosition;
	unsigned __int64 m_totalDataWritten;
	unsigned __int64 m_totalDataProcessed;
	unsigned int m_channelDataProcessedPos[12];
	unsigned int m_channelDataWrittenPos[12];
	unsigned int m_channelBufferUnderrunRefcount[12];
	unsigned int m_curDataProcessedPos;
	unsigned int m_curDataWrittenPos;
	float m_volumeLeft[12];
	float m_volumeRight[12];
	int m_group[12];
	int m_volumeReverb;
	float m_pitchMultiplier;
	bool m_useSoundVector;
	char pad[15]; // gotta have them 16-byte alignment on booleans
	cdc::Vector m_soundVector;
	bool m_loadingBusy;
	bool m_isPlaying;
	bool m_isCueing;
	bool m_playRequest;
	bool m_isWaitingForCine;
	bool m_createdInPauseMode;
};

struct SoundGroupAssignments
{
	unsigned int musicSlider;
	unsigned int sfxSlider;
	unsigned int voSlider;
	unsigned int music;
	unsigned int sfx;
	unsigned int cine;
};

struct GlobalSoundInfo
{
	char skip[0x1C];
	SoundGroupAssignments* soundGroupAssignments;
};

void InstallSoundHooks();
