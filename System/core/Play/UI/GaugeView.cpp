#include <core/Play/UI/GaugeView.hpp>
#include <core/Play/LifeGauge/LifeGauge.hpp>
#include <Siv3D.hpp>

namespace ct
{
    void GaugeView::draw() const
    {
        Transformer2D t2d(Mat3x2::Translate(m_xOffs, 0));
        constexpr double barY = 50;
        constexpr Vec2 badgePos{ 770, 490 };
        constexpr Color frameColor(100, 100, 145);

        constexpr double barWidth = 14;
        constexpr double gaugeHeight = 400;
        constexpr double barL = badgePos.x - barWidth / 2.0;

        constexpr RectF baseRect(barL, barY, barWidth, badgePos.y - barY);

        constexpr double subWidth = 8;
        constexpr double subL = barL + barWidth + 1.0;
        constexpr double subT = barY + 6;
        constexpr RectF subRect(subL, subT, subWidth, badgePos.y - barY - 10);

        constexpr Vec2 badgeFrameSize = { 42, 42 };
        const Quad badgeFrame = RectF(badgePos - badgeFrameSize / 2.0, badgeFrameSize).rotated(ToRadians(45));

        ColorF clearColor = this->getClearColor();
        clearColor.setA(0.7);

        const Color frameColor2 = clearColor;
        // 後ろフレーム
        {
            baseRect.drawFrame(0, 2, ColorF(0, 0.2));
            subRect.drawFrame(0, 2, ColorF(0, 0.2));
            badgeFrame.drawFrame(0, 2, ColorF(0, 0.2));

            baseRect.drawFrame(0, 1, frameColor);
            subRect.drawFrame(0, 1, frameColor);
            badgeFrame.drawFrame(0, 1, frameColor);
        }
        // クリアゲージ
        {
            constexpr Vec2 pos(barL, barY + 5);

            constexpr RectF gaugeRect(pos, barWidth, gaugeHeight);
            gaugeRect.draw(ColorF(0.0, 0.2));
            // ゲージ
            if (m_clearRate > 0) {
                ScopedRenderStates2D blend(BlendState::Additive);
                uint8 gaugeA = 128;
                if (m_clearRate >= 97) {
                    // AAA
                    double r = Saturate((m_clearRate - 97) / 3.0);
                    double h = 4 * 3;
                    RectF(pos + Vec2{ 0, (1 - r) * h }, barWidth, h * r).draw(Color(255, 103, 0, gaugeA));
                }
                if (m_clearRate >= 80) {
                    // Clear
                    double r = Saturate((m_clearRate - 80) / 17.0);
                    double h = 4 * 17;
                    RectF(pos + Vec2{ 0, 4 * (100 - 97) + (1 - r) * h }, barWidth, h * r).draw(Color(255, 255, 0, gaugeA));
                }
                {
                    double r = Saturate(m_clearRate / 80.0);
                    double h = 4 * 80;
                    RectF(pos + Vec2{ 0, 4 * (100 - 80) + (1 - r) * h }, barWidth, h * r).draw(Color(35, 35, 255, gaugeA));
                }
            }
            for (int32 i = 1; i < 100; ++i) {
                double y = (100 - i) * 4 + pos.y;
                bool isSep = i == 50 || i == 60 || i == 70 || i == 80 || i == 90 || i == 95 || i == 97;
                Line(pos.x + 1, y, pos.x + barWidth -1, y).draw(ColorF(0, isSep ? 1.0 : 0.4));
            }
            gaugeRect.drawFrame(1, 0.0, frameColor2);
        }
        // ライフゲージ
        {
            constexpr Vec2 pos(subL, subT + 10);
            constexpr RectF subGaugeRect(pos, subWidth, gaugeHeight);
            subGaugeRect.draw(ColorF(0.0, 0.2));

            if (m_lifeRate > 0) {
                auto [lifeColor, lifeColor2] = this->getLifeColor();

                auto life0_1 = m_lifeRate / 100.0;
                RectF(pos.x, pos.y + ((1 - life0_1) * gaugeHeight), subWidth, life0_1 * gaugeHeight)
                    .draw({ lifeColor, lifeColor, lifeColor2, lifeColor2 });
            }
            for (int32 i = 1; i < 10; ++i) {
                double y = i * 40 + pos.y;
                Line(pos.x + 1, y, pos.x + subWidth -1, y).draw(ColorF(0, 0.5));
            }
        }
        // 前フレーム
        {
            {
                constexpr RectF header(barL, barY, barWidth, 5);
                header.draw(frameColor);
            }
            {
                constexpr double footerPos = barY + 5 + gaugeHeight;
                constexpr RectF footer(barL, footerPos, barWidth, badgePos.y - footerPos);
                footer.draw(frameColor2);
            }
            {
                constexpr RectF header(subL, subT, subWidth, 10);
                header.draw(frameColor);
            }
            {
                constexpr double footerPos = subT + 10 + gaugeHeight;
                constexpr RectF footer(subL, footerPos, subWidth, badgePos.y - footerPos);
                footer.draw(frameColor);
            }
            badgeFrame.draw(frameColor2);
        }
        // バッジ
        {
            ColorF color = (m_lifeRate <= 0) ? ColorF(0.7, 1.0) : ColorF{ Palette::White };
            LifeGauge::GetBadge(m_kind).drawAt(badgePos, color);
        }
    }
    s3d::ColorF GaugeView::getClearColor() const
    {
        ColorF clearColor;
        auto p = Periodic::Triangle0_1(2.0);
        if (m_clearRate >= 97) {
            clearColor = Color(141, 128, 91);
            HSV hsv = clearColor;
            hsv.v = hsv.v + (1 - hsv.v) * p;
            clearColor = hsv;
        } else if (m_clearRate >= 95) {
            clearColor = Color(92, 92, 92);
            HSV hsv = clearColor;
            hsv.v = hsv.v + (1 - hsv.v) * p;
            clearColor = hsv;
        } else if (m_clearRate >= 90) {
            clearColor = Color(74, 44, 24);
            HSV hsv = clearColor;
            hsv.v = hsv.v + (1 - hsv.v) * p;
            clearColor = hsv;
        } else if (m_clearRate >= 80) {
            clearColor = Color(231, 0, 21);
            HSV hsv = clearColor;
            hsv.s -= -0.5 * Fmod(Scene::Time() / 2, 1.0);
            clearColor = hsv;
        } else if (m_clearRate >= 70) {
            clearColor = Color(231, 107, 0);
        } else if (m_clearRate >= 60) {
            clearColor = Color(92, 252, 255);
        } else if (m_clearRate >= 50) {
            clearColor = Color(39, 83, 231);
        } else {
            clearColor = Color(142, 142, 142);
        }
        return clearColor;
    }
    std::pair<s3d::ColorF, s3d::ColorF> GaugeView::getLifeColor() const
    {
        ColorF lifeColor;
        ColorF lifeColor2;
        double alpha = 0.8 + Periodic::Triangle0_1(1.0, m_beatRate) * 0.2;
        double sFactor = 0.5 + 0.5 * Periodic::Triangle0_1(1.0, m_beatRate);
        if (m_lifeRate <= 25) {
            lifeColor = ColorF(1.0, 0.2, 0.2, alpha);
            lifeColor2 = HSV(360.0, sFactor, 1).toColorF(alpha);
        } else if (m_lifeRate <= 50) {
            alpha = 1.0;
            lifeColor = ColorF(1.0, 1.0, 0.2, alpha);
            HSV hsv = ColorF(1.0, 0.2, 0.2, alpha);
            //hsv.s *= sFactor;
            lifeColor2 = hsv.toColorF(alpha);
        } else if (m_lifeRate < 100) {
            alpha = 1.0;
            lifeColor = ColorF(0.4, 1.0, 0.4, alpha);
            HSV hsv = ColorF(1.0, 1.0, 0.2, alpha);
            //hsv.s *= sFactor;
            lifeColor2 = hsv.toColorF(alpha);
        } else {
            lifeColor = HSV(Fmod(Scene::Time() / 2, 1.0) * 360, 0.5, 1).toColorF(alpha);
            lifeColor2 = HSV(Fmod(Scene::Time() / 2 + 0.25, 1.0) * 360, 0.5 * sFactor, 1).toColorF(alpha);
        }
        return { lifeColor, lifeColor2 };
    }
}
