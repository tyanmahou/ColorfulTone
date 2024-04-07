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
            m_clearRate = static_cast<double>(percent);
            return *this;
        }
        GaugeView& setLifeRate(float percent)
        {
            m_lifeRate = static_cast<double>(percent);
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
        s3d::ColorF getClearColor() const;
        std::pair<s3d::ColorF, s3d::ColorF> getLifeColor() const;
    private:
        double m_xOffs = 0.0;
        double m_clearRate = 100.0;
        double m_lifeRate = 100.0;
        LifeGaugeKind m_kind = {};

        double m_beatRate = 0.0;
    };
}