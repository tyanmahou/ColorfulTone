#pragma once
#include <core/Object/Object.hpp>

namespace ct
{
    class Bar :public Object
    {
    public:
        Bar(double firstCount, double speed) :Object(0, firstCount), m_speed(speed) {};
        virtual ~Bar() = default;
        virtual bool update(const PlayContext& context)override;
        virtual void diffDraw(double count, double scrollRate)const override;
        const double& getSpeed()const
        {
            return m_speed;
        }
    private:
        const double m_speed;
    };
}