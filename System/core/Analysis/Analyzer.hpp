#pragma once
#include <core/Data/NotesData/SheetMusic.hpp>

namespace ct
{
    struct AnalyzeResult
    {
        // レート
        s3d::uint64 rating;

        // 平均レート
        s3d::uint64 aveRating;
        // 局所平均レート
        s3d::uint64 localAveRating;

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
        static AnalyzeResult Analyze(const SheetMusic& sheet);
    };
}