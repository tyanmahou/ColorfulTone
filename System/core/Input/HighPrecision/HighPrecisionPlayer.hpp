#pragma once
#include <core/Input/IGameInputPlayer.hpp>

namespace ct
{
    /// <summary>
    /// 高精度入力 (FPS以上の入力判定)
    /// </summary>
    class HighPrecisionPlayer : public IGameInputPlayer
    {
    public:
        HighPrecisionPlayer();
        bool isClicked(GameInputKind kind) const override;
        bool isPressed(GameInputKind kind)const override;

        void pop(GameInputKind kind) override;
        s3d::int64 clickedMillisecOffset(GameInputKind kind) const override;

        void forceInput(GameInputKind kind, bool isLong) override;

        void update() override;
    private:
        std::queue<s3d::uint64> m_clickTimes[3];

        s3d::uint64 m_time = 0;
        s3d::uint64 m_eventIndex = 0;
    };
}