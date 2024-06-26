﻿#include <utils/Sfx/ColorFade.hpp>
#include <Siv3D.hpp>

namespace ct::Sfx
{
	void ColorFade::start(double time, const s3d::ColorF& color)
	{
		this->start(time / 2.0, time / 2.0, color);
	}
	void ColorFade::start(double fadeInTime, double fadeOutTime, const s3d::ColorF& color)
	{
		m_fadeColor = color;
		m_fadeColor.a = 0;
		m_fadeInTimer.reset(fadeInTime);
		m_fadeOutTimer.reset(fadeOutTime);
	}
	bool ColorFade::isFadeInEnd() const
	{
		return m_fadeInTimer.isEnd();
	}
	bool ColorFade::isFadeOutEnd() const
	{
		return m_fadeOutTimer.isEnd();
	}
	void ColorFade::reset()
	{
		m_fadeInTimer.reset();
		m_fadeOutTimer.reset();
		m_fadeColor.a = 0;
	}
	void ColorFade::update(double dt)
	{
		dt = m_fadeInTimer.update(dt);
		dt = m_fadeOutTimer.update(dt);

		if (!m_fadeInTimer.isEnd()) {
			m_fadeColor.a = m_fadeInTimer.rate();
		} else if (!m_fadeOutTimer.isEnd()) {
			m_fadeColor.a = m_fadeOutTimer.invRate();
		} else {
			m_fadeColor.a = 0;
		}
	}
	void ColorFade::draw() const
	{
		if (m_fadeColor.a <= 0) {
			return;
		}
		Scene::Rect().draw(m_fadeColor);
	}
}