#include <scenes/Scene/Preview/GUI/Button.hpp>
#include <Siv3D.hpp>

namespace ct::GUI
{
    Button::Button(const s3d::Font& font):
        m_font(font)
    {
    }
    s3d::RectF Button::draw(s3d::StringView text, const s3d::Vec2& pos, const s3d::Optional<s3d::Vec2>& size) const
    {
        const double height = size.has_value() ? size->y : (m_font.height() + 2 * 2);
        const auto dtext = m_font(text);
        const double width = size.has_value() ? size->x : (Math::Ceil(dtext.region(20).w) + 15);

        RectF region{ pos, {width, height} };
        if (m_enabled && region.mouseOver()) {
            region.draw(m_mouseOverBackColor);
            Cursor::RequestStyle(CursorStyle::Hand);
            if (m_mouseOverCallback) {
                m_mouseOverCallback();
            }
        }
        if (m_enabled && region.leftClicked()) {
            if (m_onClickCallback) {
                m_onClickCallback();
            }
        }
        dtext.drawAt(20, region.center(), m_enabled ? m_textColor : m_disbaledTextColor);
        return region;
    }
}
