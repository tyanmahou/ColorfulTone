#include <scenes/utils/SharedDraw/WarningInfo.hpp>
#include <Useful.hpp>
#include <Siv3D.hpp>

namespace ct::SharedDraw
{
    class WarningInfo::Impl
    {
    public:
        void setStarLv(StarLv starLv)
        {
            m_starLv = starLv;
        }
        void setColor(const s3d::Optional<s3d::Color>& color)
        {
            m_color = color;
        }
        void updateAlpha()
        {
            double targetAlpha = m_starLv == StarLv::None || m_starLv == StarLv::Other ? 0.0 : 1.0;
            double delta = Scene::DeltaTime();
            if (m_blend + delta < targetAlpha) {
                m_blend += delta;
            }
            else if (m_blend - delta > targetAlpha) {
                m_blend -= delta;
            }
            else {
                m_blend = targetAlpha;
            }
        }
        void updateNoise()
        {
            double target = m_starLv == StarLv::BlackThree ? 1.0
                : m_starLv == StarLv::BlackTwo ? 0.5
                : m_starLv == StarLv::BlackOne ? 0.25 
                : 0.0;
            double delta = Scene::DeltaTime();
            if (m_noiseRate + delta < target) {
                m_noiseRate += delta;
            } else if (m_noiseRate - delta > target) {
                m_noiseRate -= delta;
            } else {
                m_noiseRate = target;
            }
        }
        void apply(std::function<void()> draw)
        {
            updateAlpha();
            updateNoise();
            if (m_blend <= 0) {
                draw();
                return;
            }

            ColorF baseColor = m_color.value_or(this->warnColor());
            {
                Shaders::Warning()
                    .setColor(baseColor)
                    .setBlend(m_blend)
                    .setNoiseRate(m_noiseRate)
                    .setTime(Scene::Time())
                    .apply(draw);
            }
            {
                Transformer2D t2d(Mat3x2::Rotate(Math::ToRadians(-15)).translated({ -100, 140 }));
                auto drawable = FontAsset(FontName::Level)(this->warnText());
                constexpr double thick = 3;
                const double alpha = 0.2 + 0.3 * s3d::Periodic::Sine0_1(1s);
                const ColorF color = baseColor.withA(alpha * m_blend);;
                constexpr double witdh = 180;
                double move = witdh * s3d::Periodic::Sawtooth0_1(3s);
                const Vec2 pivot0{ move, 35 };
                const Vec2 pivot1{ move, 70 };
                Line({ 0, pivot0.y }, { 800, pivot0.y }).draw(thick, color);
                Line({ 0, pivot1.y }, { 800, pivot1.y }).draw(thick, color);
                for (size_t i = 0; i < 6; ++i) {
                    drawable.draw(Arg::leftCenter = Vec2{ -move + witdh * i, (pivot0.y + pivot1.y) / 2.0 }, color);
                }
                Line({ 0, 565 - pivot0.y }, { 800, 565 - pivot0.y }).draw(thick, color);
                Line({ 0, 565 - pivot1.y }, { 800, 565 - pivot1.y }).draw(thick, color);
                for (size_t i = 0; i < 6; ++i) {
                    drawable.draw(Arg::rightCenter = Vec2{ 800 - (-move + witdh * i), 565 - (pivot0.y + pivot1.y) / 2.0 }, color);
                }
            }
        }
        ColorF warnColor() const
        {
            switch (m_starLv) {
            case StarLv::AsteOne:
                return ColorF(0.5, 0.5, 1, 1);
            case StarLv::AsteTwo:
                return ColorF(1, 0.5, 1, 1);
            case StarLv::AsteThree:
                return ColorF(1, 0.5, 0, 1);
            case StarLv::WhiteOne:
            case StarLv::WhiteTwo:
            case StarLv::WhiteThree:
                return ColorF(1, 1, 0, 1);
            case StarLv::BlackOne:
            case StarLv::BlackTwo:
                return ColorF(1, 0, 0, 1);
            case StarLv::BlackThree:
                return ColorF(0.2, 1);
            default:
                return ColorF(1, 1);
            }
        }
        s3d::StringView warnText() const
        {
            switch (m_starLv) {
            case StarLv::AsteOne:
                return U"CAUTION";
            case StarLv::AsteTwo:
                return U"WARNING";
            case StarLv::AsteThree:
                return U"DANGER";
            case StarLv::WhiteOne:
            case StarLv::WhiteTwo:
                return U"WARNING";
            case StarLv::WhiteThree:
                return U"DANGER";
            case StarLv::BlackOne:
            case StarLv::BlackTwo:
            case StarLv::BlackThree:
                return U"FATAL";
            default:
                return U"";
            }
        }
    private:
        s3d::Optional<s3d::Color> m_color;
        StarLv m_starLv{};
        double m_blend = 0;
        double m_noiseRate = 0;
    };
    WarningInfo::WarningInfo()
        :m_pImpl(std::make_shared<Impl>())
    {
    }
    const WarningInfo& WarningInfo::setStarLv(StarLv starLv) const
    {
        m_pImpl->setStarLv(starLv);
        return *this;
    }
    const WarningInfo& WarningInfo::setColor(const s3d::Optional<s3d::Color>& color) const
    {
        m_pImpl->setColor(color);
        return *this;
    }
    void WarningInfo::apply(std::function<void()> drawble) const
    {
        m_pImpl->apply(drawble);
    }
}
