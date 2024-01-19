#include <utils/Clock/Clock.hpp>
#include <utils/Clock/ClockUtil.hpp>
#include <Siv3D.hpp>

namespace ct::Clock
{
    CustomClock::CustomClock(const std::function<s3d::uint64()>& func):
        m_func(func)
    {}
    s3d::uint64 CustomClock::getMicrosec()
    {
        return m_func();
    }

    CustomClockF::CustomClockF(const std::function<double()>& func):
        m_func(func)
    {}
    s3d::uint64 CustomClockF::getMicrosec()
    {
        return ClockUtil::FromSec(m_func()).count();
    }
}
