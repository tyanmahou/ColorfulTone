#include "EasingSequence.hpp"

EasingSequence::EasingSequence(std::initializer_list<Timer> timers):
	m_timers(timers)
{}

void EasingSequence::update()
{
	if (m_timers.empty())
	{
		return;
	}
	for (size_t index = 0; index < m_timers.size(); ++index)
	{
		if ((index == 0 || m_timers[index - 1].isEnd()) &&
			!m_timers[index].isActive() &&
			!m_timers[index].isEnd())
		{
			m_timers[index].start();
		}
	}
}

void EasingSequence::emplace_back(const double & start, const double & end, std::function<double(double)> easingFunction, double timeMillisec)
{
	m_timers.emplace_back(start, end, easingFunction, timeMillisec);
}

void EasingSequence::clear()
{
	m_timers.clear();
}

EasingSequence::iterator EasingSequence::begin()
{
	return m_timers.begin();
}

EasingSequence::iterator EasingSequence::end()
{
	return m_timers.end();
}

EasingSequence::const_iterator EasingSequence::cbegin() const
{
	return m_timers.cbegin();
}

EasingSequence::const_iterator EasingSequence::cend() const
{
	return m_timers.cend();
}

const EasingSequence::Timer & EasingSequence::operator[](size_t index) const
{
	return m_timers[index];
}
