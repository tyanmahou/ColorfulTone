#pragma once
#include <core/Data/NotesData/SheetMusic.hpp>

namespace ct
{
    struct AnalyzeResult
    {
        // レート
        s3d::uint64 rating;

        // 平均レート
        s3d::uint64 meanRating;
        // 局所中央レート
        s3d::uint64 medianRating;
        // 80パーセンタイルレート
        s3d::uint64 percentile80Rating;
        // 97パーセンタイルレート
        s3d::uint64 percentile97Rating;
        // 局所最大レート
        s3d::uint64 maxRating;
        // ノーツ重み補正値
        s3d::uint64 noteWeightRating;
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