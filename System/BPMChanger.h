#pragma once
#include"Object.h"
#include"MahouSoundBeat.h"

struct TempoInfo 
{
	int64 m_changeSample = 0;
	Mahou::SoundBar m_bar;
	TempoInfo(int64 sample, int64 offsetSample, BPMType bpm):
		m_changeSample(sample),
		m_bar(offsetSample,bpm)
	{

	}
};

//class BPMChanger :public Object
//{
//private:
//	BPMType m_nextBPM;
//	std::shared_ptr<Mahou::SoundBar> m_soundBar;
//public:
//	BPMChanger(
//		double firstCount,
//		BPMType nBpm,
//		std::shared_ptr<Mahou::SoundBar>& soundBar
//	);
//	virtual ~BPMChanger() = default;
//	virtual bool update(double& nowCount, double& countPerFrame, Score& score, Sound& sound)override;
//	virtual void diffDraw(double count, float scrollRate)const override;
//};