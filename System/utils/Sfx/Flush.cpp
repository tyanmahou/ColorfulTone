#include <utils/Sfx/Flush.hpp>

namespace ct::Sfx
{
	void Flush::start(double time, const s3d::ColorF& color)
	{
		m_fade.start(time, color);
	}
	void Flush::start(double fadeInTime, double fadeOutTime, const s3d::ColorF& color)
	{
		m_fade.start(fadeInTime, fadeOutTime, color);
	}
	bool Flush::isFadeInEnd() const
	{
		return m_fade.isFadeInEnd();
	}
	bool Flush::isFadeOutEnd() const
	{
		return m_fade.isFadeOutEnd();
	}
	void Flush::reset()
	{
		m_fade.reset();
	}
	bool Flush::isActive() const
	{
		return !isFadeInEnd() || !isFadeOutEnd();
	}
	void Flush::update(double dt)
	{
		m_fade.update(dt);
	}
	void Flush::draw() const
	{
		m_fade.draw();
	}
}