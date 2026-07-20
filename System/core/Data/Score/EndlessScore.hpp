#pragma once
#include <core/Play/LifeGauge/LifeGaugeKind.hpp>
#include <array>

namespace ct
{
    struct EndlessGaugeScore
    {
        s3d::uint64 clearCount = 0;
        s3d::uint64 maxCombo = 0;
    };
    struct EndlessScore
    {
        std::array<EndlessGaugeScore, static_cast<size_t>(LifeGaugeKind::Count)> gaugeData;

        EndlessGaugeScore& operator[](LifeGaugeKind gauge)
        {
            return gaugeData[static_cast<size_t>(gauge) - static_cast<size_t>(LifeGaugeKind::Min)];
        }

        const EndlessGaugeScore& operator[](LifeGaugeKind gauge) const
        {
            return gaugeData[static_cast<size_t>(gauge) -static_cast<size_t>(LifeGaugeKind::Min)];
        }
    };
}