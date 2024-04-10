#pragma once

namespace ct
{
    class SamplePosView
    {
    public:
        SamplePosView& setOffs(double x)
        {
            m_xOffs = x;
            return *this;
        }
        SamplePosView& setRate(double rate)
        {
            m_rate = rate;
            return *this;
        }
        void draw() const;
    private:
        double m_xOffs = 0.0;
        double m_rate = 0.0;
    };
}