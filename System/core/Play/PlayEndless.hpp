#pragma once
#include <memory>
#include <Siv3D/Fwd.hpp>
#include <core/Play/LifeGauge/LifeGaugeKind.hpp>

namespace  ct
{
    class EndlessData;
    class PlayEndless
    {
    public:
        void init(const EndlessData& endless, LifeGaugeKind guage) const;
    };
}