#include <utils/Addon/IntervalCounter.hpp>
#include <Siv3D.hpp>

namespace ct
{
    bool IntervalCounter::RegisterAddon(const Fps& fps)
    {
        return s3d::Addon::Register(AddonName, std::make_unique<IntervalCounter>(fps));
    }
    IntervalCounter* IntervalCounter::Instance()
    {
        return s3d::Addon::GetAddon<IntervalCounter>(AddonName);
    }
    bool IntervalCounter::IsUpdated()
    {
        return Instance()->isUpdated();
    }
    bool IntervalCounter::IsUpdatedEvery(s3d::uint64 n)
    {
        return Instance()->isUpdatedEvery(n);
    }
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
                m_isUpdateFrame = true;
            }
        }
        return true;
    }
    bool IntervalCounter::isUpdatedEvery(s3d::uint64 n) const
    {
        return m_isUpdateFrame && (m_count % n == 0);
    }
}
