#pragma once
#include <Siv3D/Fwd.hpp>

namespace ct
{
    enum ScoreJudge
    {
        Perfect,
        Great,
        Good,
        Miss,

        TERM,
    };

    s3d::StringView JudgeStr(ScoreJudge judge);
}