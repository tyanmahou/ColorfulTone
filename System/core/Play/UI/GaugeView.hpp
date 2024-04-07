#pragma once
#include <core/Play/LifeGauge/LifeGaugeKind.hpp>

namespace ct
{
    class GaugeView
    {
    public:
        GaugeView& setOffs(double x)
        {
            m_xOffs = x;
            return *this;
        }
        GaugeView& setClearRate(float percent)
        {
            m_clearRate = static_cast<double>(percent / 100.0f);
            return *this;
        }
        GaugeView& setLifeRate(float percent)
        {
            m_lifeRate = static_cast<double>(percent / 100.0f);
            return *this;
        }
        GaugeView& setGaugeKind(LifeGaugeKind kind)
        {
            m_kind = kind;
            return *this;
        }
        GaugeView& setBeatRate(double rate)
        {
            m_beatRate = rate;
            return *this;
        }
        void draw() const;

    private:
        double m_xOffs = 0.0;
        double m_clearRate = 1.0;
        double m_lifeRate = 1.0;
        LifeGaugeKind m_kind = {};

        double m_beatRate = 0.0;
    };
}