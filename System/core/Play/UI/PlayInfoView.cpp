#include <core/Play/UI/PlayInfoView.hpp>
#include <Useful.hpp>
#include <Siv3D.hpp>

namespace ct
{
    void PlayInfoView::DrawRate(double rate, const s3d::Optional<float>& subRate, const s3d::Vec2& ratePos)
    {
        const auto dt = FontAsset(FontName::Combo)(U"{:.2f}%"_fmt(rate));
        dt.draw(ratePos.x + 1, ratePos.y + 1, Palette::White);
        Vec2 br = dt.draw(ratePos.x, ratePos.y, Palette::Black).br();

        if (subRate) {
            const auto subDt = FontAsset(FontName::SubRate)(U"{:.2f}%"_fmt(*subRate));
            subDt.draw(Arg::topRight = br + Vec2{ -2 + 1, 5 + 1}, Palette::White);
            subDt.draw(Arg::topRight = br + Vec2{ -2, 5 }, Palette::Black);
        }
    }
    void PlayInfoView::DrawCombo(size_t combo, const s3d::Vec2& comboPos)
    {
        const FontAsset font(FontName::Combo);
        TextureAsset comboTex(U"combo");
        if (combo > 0) {
            comboTex.draw(comboPos.x + 121, comboPos.y - 1, Palette::White);
            comboTex.draw(comboPos.x + 120, comboPos.y - 2, Palette::Black);

            String comboText = Pad(combo, { 6, L' ' });
            FontKinetic::DeleteSpace(font, comboText, Vec2{ comboPos.x, comboPos.y }, Palette::Black, Palette::White);
        }
    }
}

