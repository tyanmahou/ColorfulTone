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
        static U StdDev(const s3d::Array<T>& ar)
        {
            if (ar.isEmpty()) {
                return 0.0;
            }
            auto stdDev = s3d::Statistics::PopulationStandardDeviation(ar.begin(), ar.end());
            if (!stdDev) {
                return 0.0;
            }
            return *stdDev;
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
                return 1.0;
            }
            auto logAr = ar.map([](const T& v) {
                return s3d::Log(v);
                });
            auto logStdDev = s3d::Statistics::PopulationStandardDeviation(logAr.begin(), logAr.end());
            if (!logStdDev) {
                return 1.0;
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

        template<class T, class U = double>
        static U GeometricAbsDev(const s3d::Array<T>& ar, double pivot = 1.0)
        {
            if (ar.isEmpty()) {
                return 1.0;
            }
            double n = static_cast<double>(ar.size());
            double logDevAbsSum = 0;
            double logPivot = s3d::Log(s3d::Abs(pivot));
            for (T v : ar) {
                double logDev = s3d::Log(s3d::Abs(static_cast<double>(v))) - logPivot;
                logDevAbsSum += s3d::Abs(logDev);
            }
            double logPivotDev = logDevAbsSum / n;
            return std::exp(logPivotDev);
        }

        template<class T, class U = double>
        static U MultiModeGeometricMean(const s3d::Array<T>& ar)
        {
            if (ar.isEmpty()) {
                return 1.0;
            }
            s3d::Array<T> multiMode = MultiMode<T>(ar);
            return GeometricMean<T, U>(multiMode);
        }

        template<class T>
        static s3d::Array<T> MultiMode(const s3d::Array<T>& ar)
        {
            s3d::HashTable<T, size_t> frequency;

            // 各要素の出現回数を数える
            for (const T& v : ar) {
                frequency[v]++;
            }

            // 最頻値の出現回数を見つける
            size_t maxFrequency = 0;
            for (const auto& pair : frequency) {
                maxFrequency = s3d::Max(maxFrequency, pair.second);
            }

            // 最頻値を見つける
            s3d::Array<T> modes;
            for (const auto& pair : frequency) {
                if (pair.second == maxFrequency) {
                    modes.push_back(pair.first);
                }
            }

            return modes;
        }
        template<class T>
        static size_t KindNum(const s3d::Array<T>& ar)
        {
            s3d::HashSet<T> set;
            for (const T& v : ar) {
                set.insert(v);
            }
            return set.size();
        }
        template<class T, class U = double>
        static U KindComplexity(const s3d::Array<T>& ar, double base = 0.1)
        {
            size_t n = ar.size();
            if (n <= 1) {
                return 0;
            }
            size_t changeCount = 0;
            s3d::int32 sign = 0;
            for (size_t index = 1; index < n; ++index) {
                if (ar[index] != ar[index - 1]) {
                    if (sign == 0) {
                        ++changeCount;
                    } else if (sign > 0 && ar[index] < ar[index - 1]) {
                        ++changeCount;
                    } else if (sign < 0 && ar[index] > ar[index - 1]) {
                        ++changeCount;
                    }

                    sign = (ar[index] > ar[index - 1]) ? 1 : -1;
                }
            }
            size_t k = KindNum(ar);
            return s3d::Pow(1.0 + base, changeCount * k / static_cast<double>(n - 1)) - 1.0;
        }
    };
}