﻿#pragma once

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
        // SRandom,

        None = RBY,
        Mirror = YBR,
        Rotate120 = BYR,
        Rotate60Mirror = RYB,
        Rotate240 = YRB,
        Rotate240Mirror = BRY,
    };
}