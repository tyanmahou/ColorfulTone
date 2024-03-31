#include <core/Input/FpsSync/FpsSyncPlayer.hpp>
#include <core/Input/PlayKey.hpp>

namespace ct
{
    FpsSyncPlayer::FpsSyncPlayer():
      m_isClicked{false, false, false}
    {
    }
    bool FpsSyncPlayer::isClicked(GameInputKind kind) const
    {
        return m_isClicked[static_cast<size_t>(kind)];
    }
    bool FpsSyncPlayer::isPressed(GameInputKind kind) const
    {
        return GetPlayKey(kind).pressed();
    }
    void FpsSyncPlayer::pop(GameInputKind kind)
    {
        m_isClicked[static_cast<size_t>(kind)] = false;
    }
    s3d::int64 FpsSyncPlayer::clickedMillisecOffset([[maybe_unused]]GameInputKind kind) const
    {
        return 0;
    }
    void FpsSyncPlayer::forceInput(GameInputKind kind, [[maybe_unused]]bool isLong)
    {
        m_isClicked[static_cast<size_t>(kind)] = true;
    }
    void FpsSyncPlayer::update()
    {
        for (size_t index = 0; index < 3; ++index) {
            m_isClicked[index] = GetPlayKey(index).down();
        }
    }
}
