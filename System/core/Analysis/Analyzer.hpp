﻿#pragma once
#include <core/Data/NotesData/SheetMusic.hpp>

namespace ct
{
    struct AnalyzeResult
    {
        // レート
        s3d::uint64 rating;

        // 平均レート
        s3d::uint64 meanRating;
        // 局所最大レート
        s3d::uint64 maxRating;
        // 局所中央レート
        s3d::uint64 medianRating;
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