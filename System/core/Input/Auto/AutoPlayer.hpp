#pragma once
#include <core/Input/IGameInputPlayer.hpp>

namespace ct
{
    /// <summary>
    /// Auto入力
    /// </summary>
    class AutoPlayer : public IGameInputPlayer
    {
        struct FrameCount
        {
            FrameCount() = default;
            FrameCount(s3d::int32 _c = 15) :
                count(_c)
            {}
            s3d::int32 count = 0;
        };
    public:
        AutoPlayer();
        bool isClicked(GameInputKind kind) const override;
        bool isPressed(GameInputKind kind)const override;

        void pop(GameInputKind kind) override;
        s3d::int64 clickedMillisecOffset(GameInputKind kind) const override;

        void forceInput(GameInputKind kind, bool isLong) override;

        void update() override;
    private:
        std::array<FrameCount, 3> m_isPressed;
    };
}