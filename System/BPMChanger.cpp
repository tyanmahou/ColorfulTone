//#include"BPMChanger.h"
//
//BPMChanger::BPMChanger(double firstCount, BPMType nBpm,std::shared_ptr<Mahou::SoundBar>& soundBar) :
//	Object(firstCount),
//	m_nextBPM(nBpm),
//	m_soundBar(soundBar)
//{};
//
//bool BPMChanger::update(double& nowCount, double& countPerFrame, Score& score, Sound& sound)
//{
//	if (!m_isActive)
//		return true;
//
//	auto count = m_count  - nowCount;
//	if (Abs(count) < Abs(count - countPerFrame))
//		count = 0;
//
//	if (count <= 0) 
//	{
//		auto tmp = 4 * sound.samplingRate() * 60 / m_nextBPM;
//		const auto sample = sound.samplesPlayed();
//
//		{
//			auto b = (*m_soundBar)(sound);
//			auto off = b.bar*tmp + tmp*b.f;
//			*m_soundBar = Mahou::SoundBar(sample - off, m_nextBPM);
//		}
//		
//			m_isActive = false;
//	}
//
//	return true;
//}
//void BPMChanger::diffDraw(double count, float scrollRate)const
//{
//	return;
//}
