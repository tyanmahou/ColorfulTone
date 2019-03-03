#pragma once
#include<Siv3D/Array.hpp>
#include<Siv3D/EasingController.hpp>

class EasingSequence
{
public:
	using Timer = s3d::EasingController<double>;
	using iterator = s3d::Array<Timer>::iterator;
	using const_iterator = s3d::Array<Timer>::const_iterator;
private:
	s3d::Array<Timer> m_timers;
public:
	EasingSequence() = default;
	EasingSequence(std::initializer_list<Timer> timers);
	~EasingSequence() = default;

	void update();
	void emplace_back(const double& start, const double& end, std::function<double(double)> easingFunction = s3d::Easing::Sine, double timeMillisec = 1000);
	void clear();

	iterator begin();
	iterator end();

	const_iterator cbegin() const;
	const_iterator cend()const;

	const Timer& operator [](size_t index) const;
};

