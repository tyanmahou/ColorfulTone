#pragma once
#include <core/Data/Score/SpecialResult.hpp>
#include <core/Play/LifeGauge/LifeGaugeKind.hpp>

namespace ct
{
    struct ScoreModel
    {
        bool isClear = 0;
        SpecialResult specialResult = SpecialResult::None;
        float clearRate = 0.0f;

        bool isLifeClear = false;
        LifeGaugeKind gauge = LifeGaugeKind::None;
    };
}
