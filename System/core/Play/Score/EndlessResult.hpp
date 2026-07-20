#pragma once
#include <core/Data/Score/EndlessScore.hpp>

namespace ct 
{

    struct EndlessResult
    {
        LifeGaugeKind gauge = LifeGaugeKind::None;
        EndlessGaugeScore score;
        bool isEnd = false;

        void init(LifeGaugeKind _gauge)
        {
            gauge = _gauge;
            score.maxCombo = 0;
            score.clearCount = 0;
            isEnd = false;
        }
    };
}