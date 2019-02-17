#pragma once
#include"MahouSoundBeat.h"

struct TempoInfo 
{
	int64 m_changeSample = 0;
	Mahou::SoundBar m_bar;
	TempoInfo(int64 sample, int64 offsetSample, BPMType bpm):
		m_changeSample(sample),
		m_bar(offsetSample,bpm)
	{}
};
