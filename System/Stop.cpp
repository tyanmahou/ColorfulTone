//#include"Stop.h"
//
//Stop::Stop(double firstCount, double stopTimeSec, std::shared_ptr<Mahou::SoundBar> soundBar, double judgeOffset) :
//	Object(firstCount, judgeOffset),
//	m_stopTimeSample(44100 * stopTimeSec),
//	m_soundBar(soundBar),
//	m_beginTimeSample(0),
//	m_isStop(false),
//	m_bpm(0),
//	m_offsetSample(0)
//{};
//
//bool Stop::update(double& nowCount, double& countPerFrame, Score& score, Sound& sound)
//{
//	if (!m_isActive)
//		return true;
//
//
//	auto count = m_count + m_judgeOffset - nowCount;
//	if (Abs(count) < Abs(count - countPerFrame))
//		count = 0;
//
//	//ストップ時にオフセットの変更
//
//	//停止初期化
//	if (!m_isStop&&count <= 0)
//	{
//		m_isStop = true;
//		m_beginTimeSample = sound.samplesPlayed();
//		m_bpm = m_soundBar->getBPM();
//		m_offsetSample = m_soundBar->getOffset();
//		auto b = (*m_soundBar)(sound);
//		*m_soundBar = Mahou::SoundBar(b);
//
//	}
//	if (m_isStop)
//	{
//		if (sound.samplesPlayed() >= m_beginTimeSample + m_stopTimeSample)
//		{
//			*m_soundBar = Mahou::SoundBar(m_offsetSample+m_stopTimeSample, m_bpm);
//			m_isActive = false;
//		}
//		//return false;
//	}
//	return true;
//}
//void Stop::draw(double nowCount, float scrollRate)const
//{
//	return;
//}
//
//void Stop::init() 
//{
//	Object::init();
//	m_isStop = false;
//}