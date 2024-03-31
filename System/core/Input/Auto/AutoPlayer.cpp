#include <core/Input/Auto/AutoPlayer.hpp>
#include <utils/Addon/IntervalCounter.hpp>

namespace ct
{
    AutoPlayer::AutoPlayer() :
        m_isPressed{ 0,0,0 }
    {
    }
    bool AutoPlayer::isClicked([[maybe_unused]]GameInputKind kind) const
    {
        // 使わないはずだけど、Pressedと同じにしておく
        return this->isPressed(kind);
    }
    bool AutoPlayer::isPressed(GameInputKind kind) const
    {
        return m_isPressed[static_cast<size_t>(kind)].count > 0;
    }
    void AutoPlayer::pop(GameInputKind kind)
    {
        --m_isPressed[static_cast<size_t>(kind)].count;
    }
    s3d::int64 AutoPlayer::clickedMillisecOffset([[maybe_unused]] GameInputKind kind) const
    {
        return 0;
    }
    void AutoPlayer::forceInput(GameInputKind kind, bool isLong)
    {
        const s3d::int32 fc = isLong ? 1 : 7;
        m_isPressed[static_cast<size_t>(kind)] = FrameCount(fc);
    }
    void AutoPlayer::update()
    {
        if (IntervalCounter::IsUpdated()) {
            for (auto&& elm : m_isPressed) {
                if (elm.count > 0) {
                    --elm.count;
                }
            }
        }
    }
}
