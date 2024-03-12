#pragma once
#include <array>
#include <Siv3D/Fwd.hpp>
#include <Siv3D/Array.hpp>
#include <core/Data/Score/SpecialResult.hpp>

namespace ct
{
    struct Score
    {
        enum  Judge
        {
            Perfect,
            Great,
            Good,
            Miss,

            TERM,
        };
        size_t	m_currentCombo;
        size_t m_maxCombo;
        std::array<size_t, Judge::TERM> m_judgeCount{ 0 };
        size_t m_fastCount = 0;
        size_t m_lateCount = 0;
        s3d::Array<Judge> m_judgeHistory;
        // ライフ(桁落ちの対策のため整数型)
        s3d::int32 m_initLife;
        s3d::int32 m_life;
        s3d::Array<s3d::int32> m_lifeHistory;

        Score();
        void add(Judge judge, s3d::int64 diff);
    };

    struct ScoreModel
    {
        bool isClear = 0;
        SpecialResult specialResult = SpecialResult::None;
        float clearRate = 0.0f;
    };
}
