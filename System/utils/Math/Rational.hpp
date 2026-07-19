#pragma once
#include <Siv3D/Fwd.hpp>

namespace ct
{
    struct Rational
    {
        using value_type = s3d::int64;
        [[nodiscard]]
        static Rational Parse(s3d::StringView numerator, s3d::StringView denominator);

        [[nodiscard]]
        static Rational ParseDecimal(s3d::StringView sv);

        value_type numerator;
        value_type denominator;

        constexpr Rational()
            :numerator(0)
            ,denominator(1)
        {}
        constexpr Rational(value_type _numerator, value_type _denominator) noexcept
        {
            s3d::int64 g = std::gcd(_numerator, _denominator);
            numerator = _numerator / g;
            denominator = _denominator / g;
        }

        [[nodiscard]]
        constexpr Rational operator *(Rational other) const noexcept
        {
            return Rational(this->numerator * other.numerator, this->denominator * other.denominator);
        }
        [[nodiscard]]
        constexpr Rational operator /(Rational other) const noexcept
        {
            return Rational(this->numerator * other.denominator, this->denominator * other.numerator);
        }

        [[nodiscard]]
        constexpr double toDouble() const
        {
            return numerator / static_cast<double>(denominator);
        }
        [[nodiscard]]
        constexpr operator double () const
        {
            return this->toDouble();
        }
    };
}