#pragma once
#include <core/Play/LifeGauge/LifeGaugeKind.hpp>

namespace ct
{
    struct EndlessScore
    {
        struct Data
        {
            size_t clearCount = 0;
            size_t maxCombo = 0;
        };
        Data gaugeData[static_cast<size_t>(LifeGaugeKind::Max)];
    };
}