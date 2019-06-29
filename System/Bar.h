#pragma once
#include"Object.h"

class Bar:public Object
{
private:
	const double m_speed;
public:
	Bar(double firstCount,double speed) :Object(firstCount),m_speed(speed){};
	virtual ~Bar() = default;
	virtual bool update(const double& nowCount, const double& countPerFrame)override;
	virtual void diffDraw(double count, float scrollRate)const override;
	const double& getSpeed()const
	{
		return m_speed;
	}
};