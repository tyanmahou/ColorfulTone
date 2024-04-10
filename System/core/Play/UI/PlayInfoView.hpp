#pragma once
#include <Siv3D/Vector2D.hpp>
#include <Siv3D/Optional.hpp>

namespace ct
{
    class PlayInfoView
    {
    public:
        /// <summary>
        /// レート
        /// </summary>
        /// <param name="rate"></param>
        /// <param name="subRate"></param>
        /// <param name="ratePos"></param>
        static void DrawRate(double rate, const s3d::Optional<double>& subRate, const s3d::Vec2& ratePos);

        /// <summary>
        /// コンボ
        /// </summary>
        /// <param name="combo"></param>
        /// <param name="comboPos"></param>
        static void DrawCombo(size_t combo, const s3d::Vec2& comboPos);
    };
}