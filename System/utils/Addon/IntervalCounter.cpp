#include <utils/Addon/IntervalCounter.hpp>
#include <Siv3D.hpp>
#include "IntervalCounter.hpp"

namespace ct
{
    IntervalCounter::IntervalCounter(const Fps& fps):
        m_interval(fps.deltaTime()),
        m_current(0)
    {
    }

    bool IntervalCounter::update()
    {
        m_isUpdateFrame = false;
        if (m_interval <= 0) {
            ++m_count;
            m_isUpdateFrame = true;
        } else {
            m_current += Scene::DeltaTime();
            if (m_current >= m_interval) {
                m_current = s3d::Fmod(m_current, m_interval);
                ++m_count;
            }
            m_isUpdateFrame = true;
        }
        return true;
    }
    bool IntervalCounter::isUpdatedMod(s3d::uint64 mod) const
    {
        return m_isUpdateFrame && m_count % mod == 0;
    }
}
