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
        constexpr Color frameColor2(60, 60, 90);

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

        // 後ろフレーム
        {
            baseRect.drawFrame(0, 2, ColorF(0, 0.2));
            subRect.drawFrame(0, 2, ColorF(0, 0.2));
            badgeFrame.drawFrame(0, 2, ColorF(0, 0.2));

            baseRect.drawFrame(0, 1, frameColor);
            subRect.drawFrame(0, 1, frameColor);
            badgeFrame.drawFrame(0, 1, frameColor);
        }
        // ゲージ
        {
            constexpr RectF gaugeRect(barL, barY + 5, barWidth, gaugeHeight);
            gaugeRect.draw(ColorF(0.0, 0.2));

            if (m_lifeRate > 0) {
                ColorF lifeColor;
                ColorF lifeColor2;
                double alpha = 0.8 + Periodic::Triangle0_1(1.0, m_beatRate) * 0.2;
                double sFactor = 0.5 + 0.5 * Periodic::Triangle0_1(1.0, m_beatRate);
                if (m_lifeRate <= 0.25) {
                    lifeColor = ColorF(1.0, 0.2, 0.2, alpha);
                    lifeColor2 = HSV(360.0, sFactor, 1).toColorF(alpha);
                } else if (m_lifeRate <= 0.5) {
                    lifeColor = ColorF(1.0, 1.0, 0.2, alpha);
                    HSV hsv = ColorF(1.0, 0.2, 0.2, alpha);
                    hsv.s *= sFactor;
                    lifeColor2 = hsv.toColorF(alpha);
                } else if (m_lifeRate < 1.0) {
                    lifeColor = ColorF(0.4, 1.0, 0.4, alpha);
                    HSV hsv = ColorF(1.0, 1.0, 0.2, alpha);
                    hsv.s *= sFactor;
                    lifeColor2 = hsv.toColorF(alpha);
                } else {
                    lifeColor = HSV(Fmod(Scene::Time() / 2, 1.0) * 360, 0.5, 1).toColorF(alpha);
                    lifeColor2 = HSV(Fmod(Scene::Time() / 2 + 0.25, 1.0) * 360, 0.5 * sFactor, 1).toColorF(alpha);
                }
                RectF(barL, barY + 5 + ((1 - m_lifeRate) * gaugeHeight), barWidth, m_lifeRate * gaugeHeight)
                    .draw({ lifeColor, lifeColor, lifeColor2, lifeColor2 });
            }
            for (int32 i = 1; i < 100; ++i) {
                double y = i * 4 + barY + 5;
                Line(barL + 1, y, barL + barWidth -1, y).draw(ColorF(0, i % 10 == 0 ? 1.0 : 0.4));
            }
            gaugeRect.drawFrame(1, 0.0, frameColor2);
        }
        // クリアゲージ
        {
            constexpr Vec2 pos(subL, subT + 10);
            constexpr RectF subGaugeRect(pos, subWidth, gaugeHeight);
            subGaugeRect.draw(ColorF(0.0, 0.2));

            double clearP = m_clearRate * 100;
            ColorF clearColor;
            if (clearP >= 97) {
                clearColor = Color(141, 128, 91);
                HSV hsv = clearColor;
                hsv.v = hsv.v + (1 - hsv.v) * Periodic::Triangle0_1(2.0);
                clearColor = hsv;
            } else if (clearP >= 95) {
                clearColor = Color(92, 92, 92);
                HSV hsv = clearColor;
                hsv.v = hsv.v + (1 - hsv.v) * Periodic::Triangle0_1(2.0);
                clearColor = hsv;
            } else if (clearP >= 90) {
                clearColor = Color(74, 44, 24);
                HSV hsv = clearColor;
                hsv.v = hsv.v + (1 - hsv.v) * Periodic::Triangle0_1(2.0);
                clearColor = hsv;
            } else if (clearP >= 80) {
                clearColor = Color(231, 0, 21);
                HSV hsv = clearColor;
                hsv.s -= -0.5 * Fmod(Scene::Time() / 2, 1.0);
                clearColor = hsv;
            } else if (clearP >= 70) {
                clearColor = Color(231, 107, 0);
            } else if (clearP >= 60) {
                clearColor = Color(92, 252, 255);
            } else if (clearP >= 50) {
                clearColor = Color(39, 83, 231);
            } else {
                clearColor = Color(142, 142, 142);
            }
            clearColor.setA(0.7);

            // ゲージ
            if (clearP > 0) {
                //ScopedRenderStates2D blend(BlendState::Additive);
                uint8 gaugeA = 128;
                if (clearP >= 97) {
                    // AAA
                    double r = Saturate((clearP - 97) / 3.0);
                    double h = 4 * 3;
                    RectF(pos + Vec2{ 0, (1 - r) * h }, subWidth, h * r).draw(Color(255, 103, 0, gaugeA));
                }
                if (clearP >= 80) {
                    // Clear
                    double r = Saturate((clearP - 80) / 17.0);
                    double h = 4 * 17;
                    RectF(pos + Vec2{ 0, 4 * (100 - 97) + (1 - r) * h }, subWidth, h * r).draw(Color(255, 255, 0, gaugeA));
                }
                {
                    double r = Saturate(clearP / 80.0);
                    double h = 4 * 80;
                    RectF(pos + Vec2{ 0, 4 * (100 - 80) + (1 - r) * h }, subWidth, h * r).draw(Color(35, 35, 255, gaugeA));
                }
            }
            for (int32 i : {50, 60, 70, 80, 90, 95, 97}) {
                double y = (100 - i) * 4 + subGaugeRect.y;
                Line(subL + 1, y, subL + subWidth - 1, y).draw(ColorF(0, 0.4));
            }
            subGaugeRect.drawFrame(1, 0, clearColor);
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
                const RectF header(subL, subRect.y, subWidth, 10);
                header.draw(frameColor);
            }
            {
                const double footerPos = subT + 10 + gaugeHeight;
                const RectF footer(subL, footerPos, subWidth, badgePos.y - footerPos);
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
}
