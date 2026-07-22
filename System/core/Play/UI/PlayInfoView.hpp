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
        static void DrawRate(double rate, const s3d::Optional<float>& subRate, const s3d::Vec2& ratePos);

        /// <summary>
        /// コンボ
        /// </summary>
        static void DrawCombo(size_t combo, const s3d::Optional<size_t>& totalCombo, const s3d::Vec2& comboPos);
    };
}