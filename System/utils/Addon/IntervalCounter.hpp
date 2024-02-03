#pragma once
#include <Siv3D/Addon.hpp>
#include <utils/Fps/Fps.hpp>

namespace ct
{
    /// <summary>
    /// 定期更新カウンタ
    /// </summary>
    class IntervalCounter : public s3d::IAddon
    {
        static constexpr auto* AddonName = U"IntervalCounter";
    public:
        static bool RegisterAddon(const Fps& fps = 60_fps);

        static IntervalCounter* Instance();

        static bool IsUpdated();
        static bool IsUpdatedEvery(s3d::uint64 n = 1);
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
        bool isUpdatedEvery(s3d::uint64 n = 1) const;

        template<class Pred>
        bool isUpdatedWhen(Pred pred)
        {
            return m_isUpdateFrame && pred(m_count);
        }
    private:
        s3d::uint64 m_count = 0;
        const double m_interval = 0;
        double m_current = 0;
        bool m_isUpdateFrame = false;
    };
}