#pragma once

namespace ct
{
    enum class Judge
    {
        Perfect = 4,
        Great = 8,//9,
        Good = 12,
    };

    s3d::int64 JudgeRange(Judge judge);

    inline consteval s3d::int64 OneFrameSample()
    {
        return static_cast<s3d::int64>(44100 / 60.0);
    }
}
