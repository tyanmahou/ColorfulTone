#pragma once

#include <Siv3D/Statistics.hpp>
#include <concepts>

namespace ct
{
    class StatisticsUtil
    {
    public:
        template<class T>
        static T Min(const s3d::Array<T>& ar)
        {
            return s3d::Statistics::Min(ar.begin(), ar.end()).value_or(T{});
        }
        template<class T>
        static T Max(const s3d::Array<T>& ar)
        {
            return s3d::Statistics::Max(ar.begin(), ar.end()).value_or(T{});
        }
        template<class T, class U = double>
        static U Mean(const s3d::Array<T>& ar)
        {
            return static_cast<U>(s3d::Statistics::Mean(ar.begin(), ar.end()));
        }
        template<class T, class U = double>
        static U Median(const s3d::Array<T>& ar)
        {
            return static_cast<U>(s3d::Statistics::Median(ar.begin(), ar.end()).value_or(0));
        }
        template<class T>
        static std::pair<double, double> IQRBounds(const s3d::Array<T>& ar, double tukey = 1.5)
        {
            if (ar.isEmpty()) {
                return {};
            }
            const auto sorted = ar.sorted();
            const size_t n = sorted.size();
            // Q1
            const size_t indexQ1 = n / 4;
            const double q1 = n % 4 == 0 ? (sorted[indexQ1 - 1] + sorted[indexQ1]) / 2.0 : sorted[indexQ1];

            // Q3
            const size_t indexQ3 = (3 * n) / 4;
            const double q3 = n % 4 == 0 ? (sorted[indexQ3 - 1] + sorted[indexQ3]) / 2.0 : sorted[indexQ3];

            // IQR
            const double iqr = q3 - q1;

            // 範囲計算
            return { q1 - tukey * iqr, q3 + tukey * iqr };
        }

        /// <summary>
        /// IQRの範囲内で平均を求める
        /// </summary>
        /// <param name="ar"></param>
        /// <param name="tukey"></param>
        template<class T, class U = double>
        static U MeanInIQRBounds(const s3d::Array<T>& ar, double tukey = 1.5)
        {
            auto [lower, upper] = IQRBounds(ar, tukey);
            auto inIQRBounds = ar.removed_if([&](const T& v){
                return v < lower || upper < v;
            });
            return Mean<U>(inIQRBounds);
        }
    };
}