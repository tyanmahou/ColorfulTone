#pragma once
#include<Siv3D.hpp>
#include"Score.h"
#include"Stop.h"
//BPM�̌^
using BPMType = double;

class Object
{
public :
	enum
	{
		RESOLUTION = 60,
	};	//�Ԋu�̒���
protected:
	const double m_count;		//�J�E���g
	double m_drawCount;

public:
	bool m_isActive;			//���݃t���O

	Object() = default;
	Object(double firstCount) :
		m_count(firstCount),
		m_isActive(true),
		m_drawCount(firstCount)
	{};
	virtual void init() { m_isActive = true; }
	virtual ~Object() {};
	virtual bool update(double& nowCount, double& countPerFrame, Score& score, Sound& sound) = 0;

	virtual void diffDraw(double count, float scrollRate)const = 0;

	void draw(double nowCount, float scrollRate)const
	{
		const double count = m_drawCount - nowCount;

		if (m_isActive)
		{
			diffDraw(count, scrollRate);
		}

	}
	const double& getCount()const { return m_count; }
	const double& getDrawCount()const { return m_drawCount; }

	//���ʒ�~���̒���
	void addStopCount(const StopRange& stopRange)
	{
		if (m_count > stopRange.m_judgeCount)
		{

			const auto range = std::min(m_count - stopRange.m_judgeCount, stopRange.m_rangeCount);

			m_drawCount -= range;
		}
	}

	static bool CanDraw(const Vec2& pos);
};
