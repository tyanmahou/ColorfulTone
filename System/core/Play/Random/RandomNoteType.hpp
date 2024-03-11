#pragma once

namespace ct
{
    enum class RandomNoteType
    {
        RBY,
        YBR,
        BYR,
        RYB,
        YRB,
        BRY,
        Random,
        BarRandom,
        SRandom,

        None = RBY,
        Mirror = YBR,
        Rotate120 = BYR,
        Rotatee120Mirror = RYB,
        Rotate240 = YRB,
        Rotate240Mirror = BRY,
    };
}