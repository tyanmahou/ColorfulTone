#pragma once

#include<Siv3D.hpp>
namespace Mahou
{
	struct BarCount
	{
		int64 bar;

		double f;
		BarCount(int64 bar, double f) :bar(bar), f(f) {}
	};
	class SoundBar
	{
	private:
		BPMType m_bpm;
		int64 m_offsetSample;

	public:
		SoundBar() :
			m_bpm(0),
			m_offsetSample(0)
		{}
		SoundBar(int64 offsetSample, BPMType bpm) :
			m_bpm(bpm),
			m_offsetSample(offsetSample)
		{}
		SoundBar(Mahou::BarCount barCount) :
			m_bpm(0),
			m_offsetSample(0)
		{}

		BarCount operator()(const Sound& sound)const
		{
			//1小節のサンプル数
			const int64 samplePerBar = 60.0f * 4.0f / m_bpm * 44100.0f;
#ifdef VIEWER_MODE
			const int64 currentSample = sound.streamPosSample()- m_offsetSample;
#else
			const int64 currentSample = sound.samplesPlayed() - m_offsetSample;
			
#endif

			const int64 currentBar = currentSample / samplePerBar;
			const int64 lastSample = currentSample%samplePerBar;
			const double f = (double)lastSample / samplePerBar;

			return BarCount{ currentBar,f };
		}
		const int64 getOffset()const { return m_offsetSample; }
		const BPMType& getBPM()const { return m_bpm; }
	};


}
