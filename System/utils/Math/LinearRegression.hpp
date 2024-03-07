#pragma once
#include <Siv3D/Array.hpp>

namespace ct
{
    /// <summary>
    /// 線形回帰
    /// </summary>
    class LinearRegression
    {
    public:
        LinearRegression() = default;

        void fit(const s3d::Array<std::pair<double, double>>& data)
        {
            double sumX = 0, sumY = 0, sumXY = 0, sumX2 = 0;

            size_t n = data.size();
            for (size_t i = 0; i < n; ++i) {
                sumX += data[i].first;
                sumY += data[i].second;
                sumXY += data[i].first * data[i].second;
                sumX2 += data[i].first * data[i].first;
            }

            m_a = (n * sumXY - sumX * sumY) / (n * sumX2 - sumX * sumX);
            m_b = (sumY - m_a * sumX) / n;
        }

        double predict(double x) const
        {
            return m_a * x + m_b;
        }
    private:
        double m_a = 0;
        double m_b = 0;
    };
}