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
        void updateAlpha()
        {
            double targetAlpha = m_starLv == StarLv::None || m_starLv == StarLv::Other ? 0.0 : 1.0;
            double delta = Scene::DeltaTime();
            if (m_alpha + delta < targetAlpha) {
                m_alpha += delta;
            }
            else if (m_alpha - delta > targetAlpha) {
                m_alpha -= delta;
            }
            else {
                m_alpha = targetAlpha;
            }
        }
        void apply(std::function<void()> draw)
        {
            updateAlpha();
            if (m_alpha <= 0) {
                draw();
                return;
            }

            ColorF baseColor = this->warnColor();
            {
                const double alpha = s3d::Periodic::Triangle0_1(2s);
                const ColorF color = baseColor.withA(alpha * m_alpha);
                Shaders::Blend()
                    .setColor(color)
                    .apply(draw);
            }
            {
                Transformer2D t2d(Mat3x2::Rotate(Math::ToRadians(-15)).translated({ -100, 140 }));
                auto drawable = FontAsset(FontName::Level)(this->warnText());
                constexpr double thick = 3;
                const double alpha = 0.2 + 0.3 * s3d::Periodic::Sine0_1(1s);
                const ColorF color = baseColor.withA(alpha * m_alpha);;
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
            case StarLv::AsteTwo:
            case StarLv::AsteThree:
                return ColorF(1, 0, 1, 1);
            case StarLv::WhiteOne:
            case StarLv::WhiteTwo:
            case StarLv::WhiteThree:
                return ColorF(1, 1, 0, 1);
            case StarLv::BlackOne:
            case StarLv::BlackTwo:
                return ColorF(1, 0, 0, 1);
            case StarLv::BlackThree:
                return ColorF(0, 0, 0, 1);
            default:
                return ColorF(1, 1);
            }
        }
        s3d::StringView warnText() const
        {
            switch (m_starLv) {
            case StarLv::AsteOne:
            case StarLv::AsteTwo:
            case StarLv::AsteThree:
                return U"CAUTION";
            case StarLv::WhiteOne:
            case StarLv::WhiteTwo:
            case StarLv::WhiteThree:
                return U"WARNING";
            case StarLv::BlackOne:
            case StarLv::BlackTwo:
                return U"DANGER";
            case StarLv::BlackThree:
                return U"FATAL";
            default:
                return U"";
            }
        }
    private:
        StarLv m_starLv;
        double m_alpha = 0;
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
    void WarningInfo::apply(std::function<void()> drawble) const
    {
        m_pImpl->apply(drawble);
    }
}
