﻿#include <utils/Easing/EasingSequence.hpp>
#include <Siv3D.hpp>

namespace ct
{
	EasingGroup::EasingGroup(std::initializer_list<Timer> timers) :
		m_timers(timers)
	{}

	void EasingGroup::push_back(const Timer& timer)
	{
		m_timers.push_back(timer);
	}

	void EasingGroup::emplace_back(const double& start, const double& end, double easingFunction(double), double timeMillisec)
	{
		m_timers.emplace_back(start, end, easingFunction, timeMillisec);
	}

	void EasingGroup::clear()
	{
		m_timers.clear();
	}

	bool EasingGroup::isActive() const
	{
		return m_timers.any([](const Timer& timer) {
			return timer.isMoving();
		});
	}

	bool EasingGroup::isEnd() const
	{
		return m_timers.all([](const Timer& timer) {
			return timer.done();
			});
	}

	void EasingGroup::start()
	{
		for (auto&& timer : m_timers) {
			timer.start();
		}
	}

	EasingGroup::iterator EasingGroup::begin()
	{
		return m_timers.begin();
	}

	EasingGroup::iterator EasingGroup::end()
	{
		return m_timers.end();
	}

	EasingGroup::const_iterator EasingGroup::cbegin() const
	{
		return m_timers.cbegin();
	}

	EasingGroup::const_iterator EasingGroup::cend() const
	{
		return m_timers.cend();
	}

	size_t EasingGroup::size() const
	{
		return m_timers.size();
	}

	const EasingGroup::Timer& EasingGroup::operator[](size_t index) const
	{
		return m_timers[index];
	}
	//--------------------------------------------------------------------------
	void EasingSequence::update()
	{
		if (m_groups.empty()) {
			return;
		}
		auto prevIt = m_groups.begin();
		for (auto it = m_groups.begin(); it != m_groups.end(); ++it) {
			auto& easings = it->second;
			if ((it == prevIt || prevIt->second.isEnd()) &&
				!easings.isActive() &&
				!easings.isEnd()) {
				easings.start();
			}
			prevIt = it;
		}
	}

	void EasingSequence::clear()
	{
		m_groups.clear();
	}

	EasingSequence::iterator EasingSequence::begin()
	{
		return m_groups.begin();
	}
	EasingSequence::iterator EasingSequence::end()
	{
		return m_groups.end();
	}

	EasingSequence::const_iterator EasingSequence::cbegin() const
	{
		return m_groups.cbegin();
	}

	EasingSequence::const_iterator EasingSequence::cend() const
	{
		return m_groups.cend();
	}
	void EasingSequence::regist(const s3d::String& key, const EasingGroup::Timer& easing, s3d::int32 group)
	{
		if (m_groups.find(group) == m_groups.end()) {
			// インスタンスの生成
			m_groups[group] = EasingGroup();
		}
		m_groups[group].push_back(easing);
		m_dictionary[key] = { group, m_groups[group].size() - 1 };
	}
	const EasingGroup& EasingSequence::operator[](s3d::int32 group) const
	{
		return m_groups.at(group);
	}

	const EasingGroup::Timer& EasingSequence::operator[](const s3d::String& key) const
	{
		const auto& [groupIndex, index] = m_dictionary.at(key);
		return m_groups.at(groupIndex)[index];
	}

}
