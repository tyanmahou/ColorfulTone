#pragma once

namespace ct
{
    /// <summary>
    /// ゲージ種類
    /// </summary>
    enum class LifeGaugeKind
    {
        Invincible = -3,
        Safety = -2,
        Support = -1,
        Normal = 0,
        Challenge = 1,
        Danger = 2,
        SuddenDeath = 3,
    };
}