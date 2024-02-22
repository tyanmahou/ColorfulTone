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
        static U GeometricMean(const s3d::Array<T>& ar)
        {
            if (ar.isEmpty()) {
                return 1.0;
            }
            T logSum = 0;
            for (const T& v : ar) {
                logSum += s3d::Log(v);
            }
            return static_cast<U>(s3d::Exp(logSum / ar.size()));
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
        template<class T>
        static s3d::Array<T> FilterdInIQRBounds(const s3d::Array<T>& ar, double tukey = 1.5)
        {
            auto [lower, upper] = IQRBounds(ar, tukey);
            return ar.removed_if([&](const T& v) {
                return v < lower || upper < v;
                });
        }
        template<class T, class U = double>
        static U GeometricMeanInIQRBounds(const s3d::Array<T>& ar, double tukey = 1.5)
        {
            return GeometricMean<U>(FilterdInIQRBounds(ar, tukey));
        }
        template<class T, class U = double>
        static U GeometricStdDev(const s3d::Array<T>& ar)
        {
            if (ar.isEmpty()) {
                return U{};
            }
            auto logAr = ar.map([](const T& v) {
                return s3d::Log(v);
                });
            auto logStdDev = s3d::Statistics::PopulationStandardDeviation(logAr.begin(), logAr.end());
            if (!logStdDev) {
                return U{};
            }
            return static_cast<U>(s3d::Exp(*logStdDev));
        }

        template<class T, class U = double>
        static U Percentile(const s3d::Array<T>& ar, double percent)
        {
            if (ar.isEmpty()) {
                return U{};
            }
            const auto sorted = ar.sorted();
            const size_t n = sorted.size();

            // インデックスを計算
            double index = (percent / 100.0) * (n - 1);

            // インデックスの整数部分と小数部分を取得
            size_t lowerIndex = static_cast<size_t>(index);
            double fraction = index - lowerIndex;
            // パーセンタイルを計算
            if (lowerIndex >= n - 1) {
                return sorted.back();
            } else {
                return sorted[lowerIndex] + fraction * (sorted[lowerIndex + 1] - sorted[lowerIndex]);
            }
        }
    };
}