﻿#pragma once
#include <core/Input/IGameInputPlayer.hpp>

namespace ct
{
    /// <summary>
    /// FPS同期の入力
    /// </summary>
    class FpsSyncPlayer : public IGameInputPlayer
    {
    public:
        FpsSyncPlayer();
        bool isClicked(GameInputKind kind) const override;
        bool isPressed(GameInputKind kind)const override;

        void pop(GameInputKind kind) override;
        s3d::int64 clickedMillisecOffset(GameInputKind kind) const override;

        void forceInput(GameInputKind kind, bool isLong) override;

        void update() override;
    private:
        bool m_isClicked[3];
    };
}