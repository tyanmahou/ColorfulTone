#pragma once
#include<unordered_map>
#include<map>
#include<Siv3D/Array.hpp>
#include<Siv3D/EasingController.hpp>
#include<Siv3D/String.hpp>

class EasingGroup
{
public:
	using Timer = s3d::EasingController<double>;
	using iterator = s3d::Array<Timer>::iterator;
	using const_iterator = s3d::Array<Timer>::const_iterator;
private:
	s3d::Array<Timer> m_timers;
public:
	EasingGroup() = default;
	EasingGroup(std::initializer_list<Timer> timers);
	~EasingGroup() = default;

	void push_back(const Timer& timer);
	void emplace_back(const double& start, const double& end, std::function<double(double)> easingFunction = s3d::Easing::Sine, double timeMillisec = 1000);
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
};

class EasingSequence
{
public:
	using Groups = std::map<int, EasingGroup>;
	using iterator = Groups::iterator;
	using const_iterator = Groups::const_iterator;
private:
	Groups m_groups;
	std::unordered_map<s3d::String, std::pair<int, size_t>> m_dictionary;
public:
	EasingSequence() = default;
	~EasingSequence() = default;

	void update();
	void clear();

	iterator begin();
	iterator end();

	const_iterator cbegin() const;
	const_iterator cend()const;

	void regist(const s3d::String& key,const EasingGroup::Timer& easing, int group = 0);
	const EasingGroup& operator [](int group) const;
	const EasingGroup::Timer& operator [](const s3d::String& key) const;
};

