#include"TextObject.h"
#include"PlayMusicGame.h"
#include"UtilPP.hpp"

bool TextObject::update(const double& nowCount, const double& countPerFrame)
{
	const Sound*const pSound = PlayMusicGame::CurrentSound();

	if (!m_isActive || !pSound)
		return true;
#ifdef VIEWER_MODE
	const auto sample = pSound->streamPosSample();
#else
	const auto sample = pSound->samplesPlayed();
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
	MAYBE_UNUSED(count);
	MAYBE_UNUSED(scrollRate);

	if (m_isDraw)
	{
		const auto win = Window::Size();
		PutText(m_msg).at(
			static_cast<double>(win.x)/2.0,
			static_cast<double>(win.y)/3.0
		);
	}
}

void TextObject::init()
{
	Object::init();
	m_isDraw = false;
}
