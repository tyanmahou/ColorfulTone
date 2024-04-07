#pragma once

namespace ct
{
    /// <summary>
    /// ゲージ種類
    /// </summary>
    enum class LifeGaugeKind : s3d::int32
    {
        None = -99999,
        Invincible = -3,
        Safety = -2,
        Support = -1,
        Normal = 0,
        Challenge = 1,
        Danger = 2,
        SuddenDeath = 3,

        Min = -3,
        Max = 3,
    };
}