#pragma once
#include <Siv3D/Vector2D.hpp>

namespace ct::Constants
{
    // アプリ名
    inline constexpr auto* const AppName = U"ColorfulTone";
    // 解像度
    template<class Type>
    inline constexpr s3d::Vector2D<Type> AppResolution_v{ 800, 600 };
    inline constexpr s3d::Size AppResolution = AppResolution_v<s3d::int32>.asPoint();
    inline constexpr s3d::Vec2 AppResolutionF = AppResolution_v<double>;
}