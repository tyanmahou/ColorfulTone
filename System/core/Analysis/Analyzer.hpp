#pragma once
#include <core/Data/NotesData/SheetMusic.hpp>

namespace ct
{
    struct AnalyzeResult
    {
        // レート
        s3d::uint64 rating;

        // 平均レート
        double aveRating;
        // 局所平均レート
        double localAveRating;

        // 停止レート
        double stopRating;

        // BPM変化レート
        double bpmRating;

        // 速度変化レート
        double speedRating;
    };
    /// <summary>
    /// アナライザー
    /// </summary>
    class Analyzer
    {
    public:
        AnalyzeResult analyze(const SheetMusic& sheet);
    };
}