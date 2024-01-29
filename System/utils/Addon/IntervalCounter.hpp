#pragma once
#include <Siv3D/Addon.hpp>
#include <utils/Fps/Fps.hpp>

namespace ct
{
    class IntervalCounter : public s3d::IAddon
    {
    public:
        IntervalCounter(const Fps& fps = 60_fps);
    protected:
        bool update() override;
    private:
        s3d::uint64 count() const
        {
            return m_count;
        }
        bool isUpdated() const
        {
            return m_isUpdateFrame;
        }
        bool isUpdatedMod(s3d::uint64 mod = 1) const;

        template<class Pred>
        bool isUpdatedWhen(Pred pred)
        {
            return m_isUpdateFrame && pred(m_count);
        }
    private:
        s3d::uint64 m_count = 0;
        double m_interval = 0;
        double m_current = 0;
        bool m_isUpdateFrame = false;
    };
}