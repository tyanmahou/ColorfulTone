#include <utils/Math/Rational.hpp>
#include <Siv3D/StringView.hpp>
#include <Siv3D/Parse.hpp>

namespace ct
{
    Rational Rational::Parse(s3d::StringView numerator, s3d::StringView denominator)
    {
        return Rational::ParseDecimal(numerator) / Rational::ParseDecimal(denominator);
    }
    Rational Rational::ParseDecimal(s3d::StringView sv)
    {
        auto dot = sv.indexOf(U'.');

        // 小数点なし
        if (dot == s3d::StringView::npos) {
            return { s3d::ParseInt<value_type>(sv), 1};
        }

        s3d::StringView integerPar = sv.substr(0, dot);
        s3d::StringView decimalPart = sv.substr(dot + 1);
        value_type denominator = 1;
        for (size_t i = 0; i < decimalPart.size(); ++i)
            denominator *= 10;
        s3d::int64 numerator = s3d::ParseInt<value_type>(integerPar) * denominator
            + s3d::ParseInt<value_type>(decimalPart);
        return Rational(numerator, denominator);
    }
}
