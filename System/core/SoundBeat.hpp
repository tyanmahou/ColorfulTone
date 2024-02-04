#pragma once
#include <Siv3D/Audio.hpp>
#include <Siv3D/Types.hpp>
#include <core/Types.hpp>

namespace ct
{
	inline s3d::int64 GetSamplePos(const s3d::Audio& sound)
	{
		return sound.posSample();
		//#ifdef VIEWER_MODE
		//		return sound.streamPosSample();
		//#else
		//		return sound.samplesPlayed();
		//
		//#endif
	}
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
		SoundBar([[maybe_unused]]BarCount barCount) :
			m_bpm(0),
			m_offsetSample(0)
		{}

		BarCount operator()(const s3d::int64 samples)const
		{
			//1小節のサンプル数
			const s3d::int64 samplePerBar = static_cast<s3d::int64>(60.0 * 4.0 / m_bpm * 44100);
			const s3d::int64 currentSample = samples - m_offsetSample;

			const s3d::int64 currentBar = currentSample / samplePerBar;
			const s3d::int64 lastSample = currentSample % samplePerBar;
			const double f = static_cast<double>(lastSample) / samplePerBar;

			return BarCount{ currentBar,f };
		}
		BarCount operator()(const s3d::Audio& sound, s3d::int64 ajust = 0)const
		{
			return (*this)(GetSamplePos(sound) + ajust);
		}
		const s3d::int64 getOffset()const { return m_offsetSample; }
		const BPMType& getBPM()const { return m_bpm; }
	};

}
