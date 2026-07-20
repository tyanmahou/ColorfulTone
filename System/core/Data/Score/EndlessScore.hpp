#pragma once
#include <core/Play/LifeGauge/LifeGaugeKind.hpp>
#include <array>

namespace ct
{
    struct EndlessGaugeScore
    {
        size_t clearCount = 0;
        size_t maxCombo = 0;
    };
    struct EndlessScore
    {
        std::array<EndlessGaugeScore, static_cast<size_t>(LifeGaugeKind::Count)> gaugeData;
    };
}