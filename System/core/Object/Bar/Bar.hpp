#pragma once
#include <core/Object/Object.hpp>

namespace ct
{
    class Bar :public Object
    {
    public:
        Bar(s3d::int64 timingSample, double firstCount, double speed) :
            Object(timingSample, firstCount), m_speed(speed) 
        {};
        virtual ~Bar() = default;
        virtual bool update(const PlaybackState& state)override;
        virtual void diffDraw(double count, double scrollRate)const override;
        const double& getSpeed()const
        {
            return m_speed;
        }
        ObjectOrder getOrder(s3d::int64 sample) const override
        {
            return ObjectOrder::Calc(this, sample);
        }
    private:
        const double m_speed;
    };
}