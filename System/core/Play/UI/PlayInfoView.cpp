#include <core/Play/UI/PlayInfoView.hpp>
#include <Useful.hpp>
#include <Siv3D.hpp>

namespace ct
{
    void PlayInfoView::DrawRate(double rate, const s3d::Optional<double>& subRate, const s3d::Vec2& ratePos)
    {
        const FontAsset font(FontName::Combo);
        font(U"{:.2f}%"_fmt(rate)).draw(ratePos.x + 1, ratePos.y + 1, Palette::White);
        font(U"{:.2f}%"_fmt(rate)).draw(ratePos.x, ratePos.y, Palette::Black);
    }
    void PlayInfoView::DrawCombo(size_t combo, const s3d::Vec2& comboPos)
    {
        const FontAsset font(FontName::Combo);
        TextureAsset comboTex(U"combo");
        if (combo) {
            comboTex.draw(comboPos.x + 121, comboPos.y - 1, Palette::White);
            comboTex.draw(comboPos.x + 120, comboPos.y - 2, Palette::Black);

            String comboText = Pad(combo, { 6, L' ' });
            FontKinetic::DeleteSpace(font, comboText, Vec2{ comboPos.x, comboPos.y }, Palette::Black, Palette::White);
        }
    }
}

