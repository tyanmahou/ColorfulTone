#include"TextObject.h"
#include<Siv3D.hpp>

bool TextObject::update(double& nowCount, double& countPerFrame, Score& score, s3d::Sound& sound)
{
	if (!m_isActive)
		return true;

#ifdef VIEWER_MODE
	const auto sample = sound.streamPosSample();
#else
	const auto sample = sound.samplesPlayed();
#endif


	auto count = m_count  - nowCount;
	if (Abs(count) < Abs(count - countPerFrame))
		count = 0;

	//ストップ時にオフセットの変更

	//停止初期化
	if (!m_isDraw&&count <= 0)
	{
		m_isDraw = true;
		m_beginTimeSample = sample;
	}
	if (m_isDraw)
	{
		if (sample >= m_beginTimeSample + m_drawTimeSample)
		{
			m_isDraw = false;
			m_isActive = false;
		}
	}
	return true;
}
void TextObject::diffDraw(double count, float scrollRate)const
{
	if (m_isDraw)
		FontAsset(L"TextObject")(m_msg).drawCenter(400, 200);
}

void TextObject::init()
{
	Object::init();
	m_isDraw = false;
}
