#include <scenes/Scene/Preview/GUI/Slider.hpp>
#include <Siv3D.hpp>

namespace ct::GUI
{
    Slider::Slider(double& value):
        m_value(value)
    {
    }
    s3d::RectF Slider::draw(const s3d::Vec2& posTL) const
    {
        const double width = m_width;
        const RectF region{ posTL, width, m_height };
        Vec2 center = region.center();
        const double sliderRegionX0 = (region.x + 8);
        const double sliderRegionX1 = (region.x + region.w - 8);
        const double sliderRegionW = (sliderRegionX1 - sliderRegionX0);

        const double actualSliderRegionX0 = (sliderRegionX0 + 5);
        const double actualSliderRegionX1 = (sliderRegionX1 - 5);
        const double actualSliderRegionW = (actualSliderRegionX1 - actualSliderRegionX0);

        const RectF sliderRect{ Arg::leftCenter(sliderRegionX0, center.y), sliderRegionW, 6 };
        const s3d::RoundRect baseRoundRect = sliderRect.rounded(2);
        const double previousValue = m_value;
        m_value = Saturate(m_value);

        const double fill = m_value;
        const RectF fillRect{ sliderRect.pos, sliderRect.w * fill, sliderRect.h };
        const s3d::RoundRect fillRoundRect = fillRect.rounded(2.0);

        const RectF smallRect{ Arg::center(actualSliderRegionX0 + actualSliderRegionW * fill, center.y), 10, 20 };
        const s3d::RoundRect smallRoundRect = smallRect.rounded(2);
        const bool mouseOver = (m_enabled && smallRect.mouseOver());

        if (m_enabled) {
            baseRoundRect.draw(m_baseColor);
            fillRoundRect.draw({ 0.35, 0.7, 1.0 });
            smallRoundRect
                .draw(mouseOver ? ColorF{ 0.9, 0.95, 1.0 } : ColorF{ 1.0 })
                .drawFrame(1, ColorF{ 0.33 });
        } else {
            baseRoundRect.draw(m_baseColor);
            fillRoundRect.draw({ 0.35, 0.7, 1.0, 0.5 });
            smallRoundRect
                .draw(Palette::Gray)
                .drawFrame(1, ColorF{ 0.67 });
        }

        const RectF sliderRectExtended = sliderRect.stretched(4, 12);
        if (m_enabled && Cursor::OnClientRect() && (sliderRectExtended.mouseOver() || smallRect.mouseOver())) {
            Cursor::RequestStyle(CursorStyle::Hand);
        }

        if (m_enabled && Cursor::OnClientRect() && sliderRectExtended.leftPressed()) {
            const double pos = (Cursor::PosF().x - actualSliderRegionX0);
            const double posN = Math::Saturate(pos / actualSliderRegionW);
            m_value = posN;
        }

        if (m_value != previousValue) {
            if (m_onChangeValue) {
                m_onChangeValue(m_value);
            }
        }
        return region;
    }
}
