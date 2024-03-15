#pragma once
#include <Siv3D/StringView.hpp>

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

    inline constexpr s3d::StringView ToRandomNoteTypeStr(RandomNoteType type)
    {
        switch (type) {
        case RandomNoteType::Mirror:
            return U"MIRROR";
        case RandomNoteType::Rotate120:
            return U"ROTATE120";
        case RandomNoteType::Rotatee120Mirror:
            return U"ROTATE120 MIRROR";
        case RandomNoteType::Rotate240:
            return U"ROTATE240";
        case RandomNoteType::Rotate240Mirror:
            return U"ROTATE240 MIRROR";
        case RandomNoteType::Random:
            return U"RANDOM";
        case RandomNoteType::BarRandom:
            return U"BAR RANDOM";
        case RandomNoteType::SRandom:
            return U"PURE RANDOM";
        default:
            return U"DEFAULT";
        }
    }
}