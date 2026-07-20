#pragma once
#include <Siv3D/Fwd.hpp>
#include <Siv3D/Array.hpp>
#include <core/Play/LifeGauge/LifeGaugeKind.hpp>
#include <core/Play/Score/ScoreJudge.hpp>

namespace ct 
{
    struct PlayingScore
    {
        LifeGaugeKind m_gauge;

        size_t m_currentCombo;
        size_t m_maxCombo;
        size_t m_currentTotalCombo;
        size_t m_maxTotalCombo;

        std::array<size_t, ScoreJudge::TERM> m_judgeCount{ 0 };
        size_t m_fastCount = 0;
        size_t m_lateCount = 0;
        s3d::Array<ScoreJudge> m_judgeHistory;
        // ライフ(桁落ちの対策のため整数型)
        s3d::int32 m_initLife;
        s3d::int32 m_life;
        s3d::Array<s3d::int32> m_lifeHistory;

        PlayingScore();
        PlayingScore(LifeGaugeKind gauge);
        void add(ScoreJudge judge, s3d::int64 diff);

        size_t judgeCountTotal() const;
    };
}