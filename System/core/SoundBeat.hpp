#pragma once
#include <Siv3D/Audio.hpp>
#include <Siv3D/Types.hpp>
#include <core/Types.hpp>

namespace ct
{
	struct BarCount
	{
		s3d::int64 bar;

		double f;
		BarCount(s3d::int64 bar, double f) :bar(bar), f(f) {}
	};
	class SoundBar
	{
	private:
		BPMType m_bpm;
		s3d::int64 m_offsetSample;

	public:
		SoundBar() :
			m_bpm(0),
			m_offsetSample(0)
		{}
		SoundBar(s3d::int64 offsetSample, BPMType bpm) :
			m_bpm(bpm),
			m_offsetSample(offsetSample)
		{}
		SoundBar(BarCount barCount) :
			m_bpm(0),
			m_offsetSample(0)
		{}

		BarCount operator()(const s3d::Audio& sound)const
		{
			//1小節のサンプル数
			const s3d::int64 samplePerBar = 60.0f * 4.0f / m_bpm * sound.sampleRate(); // 44100
			const s3d::int64 currentSample = sound.posSample() - m_offsetSample;
//#ifdef VIEWER_MODE
//			const s3d::int64 currentSample = sound.streamPosSample() - m_offsetSample;
//#else
//			const s3d::int64 currentSample = sound.samplesPlayed() - m_offsetSample;
//
//#endif
			const s3d::int64 currentBar = currentSample / samplePerBar;
			const s3d::int64 lastSample = currentSample % samplePerBar;
			const double f = (double)lastSample / samplePerBar;

			return BarCount{ currentBar,f };
		}
		const s3d::int64 getOffset()const { return m_offsetSample; }
		const BPMType& getBPM()const { return m_bpm; }
	};


}
