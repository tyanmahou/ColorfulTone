#pragma once
#include <Siv3D/Math.hpp>

namespace ct
{
    class ErpUtil
    {
    public:
        static inline constexpr double InvEerp(double a, double b, double value) noexcept
        {
            return s3d::Log(value / a) / s3d::Log(b / a);
        }
    };
}