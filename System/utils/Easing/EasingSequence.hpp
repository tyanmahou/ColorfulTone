#pragma once
#include <Siv3D/Array.hpp>
#include <Siv3D/HashTable.hpp>
#include <Siv3D/EasingAB.hpp>
#include <Siv3D/String.hpp>

namespace ct
{
	class EasingGroup
	{
	public:
		using Timer = s3d::EasingAB<double>;
		using iterator = s3d::Array<Timer>::iterator;
		using const_iterator = s3d::Array<Timer>::const_iterator;
	public:
		EasingGroup() = default;
		EasingGroup(std::initializer_list<Timer> timers);
		~EasingGroup() = default;

		void push_back(const Timer& timer);
		void emplace_back(const double& start, const double& end, double easingFunction(double) = s3d::Easing::Sine, double timeMillisec = 1000);
		void clear();
		bool isActive()const;
		bool isEnd()const;

		void start();

		iterator begin();
		iterator end();

		const_iterator cbegin() const;
		const_iterator cend()const;

		size_t size()const;
		const Timer& operator [](size_t index) const;
	private:
		s3d::Array<Timer> m_timers;
	};

	class EasingSequence
	{
	public:
		using Groups = std::map<s3d::int32, EasingGroup>;
		using iterator = Groups::iterator;
		using const_iterator = Groups::const_iterator;
	public:
		EasingSequence() = default;
		~EasingSequence() = default;

		void update();
		void clear();

		iterator begin();
		iterator end();

		const_iterator cbegin() const;
		const_iterator cend()const;

		void regist(const s3d::String& key, const EasingGroup::Timer& easing, s3d::int32 group = 0);
		const EasingGroup& operator [](s3d::int32 group) const;
		const EasingGroup::Timer& operator [](const s3d::String& key) const;
	private:
		Groups m_groups;
		s3d::HashTable<s3d::String, std::pair<int32, size_t>> m_dictionary;
	};
}
