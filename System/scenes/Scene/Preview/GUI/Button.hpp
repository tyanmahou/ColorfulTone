#pragma once
#include <Siv3D/Font.hpp>

namespace ct::GUI
{
    class Button
    {
    public:
        Button(const s3d::Font& font);

        Button& setEnabled(bool enabled)
        {
            m_enabled = enabled;
            return *this;
        }
        Button& setFontSize(s3d::Optional<double>&& fontSize)
        {
            m_fontSize = std::move(fontSize);
            return *this;
        }
        Button& setMouseOverBackColor(const s3d::ColorF& color)
        {
            m_mouseOverBackColor = color;
            return *this;
        }
        Button& setTextColor(const s3d::ColorF& color)
        {
            return this->setTextColor(color, color);
        }
        Button& setTextColor(const s3d::ColorF& enabled, const s3d::ColorF& disabled)
        {
            m_textColor = enabled;
            m_disbaledTextColor = disabled;
            return *this;
        }
        Button& setOnClick(std::function<void()>&& callback)
        {
            m_onClickCallback = std::move(callback);
            return *this;
        }
        Button& setMouseOver(std::function<void()>&& callback)
        {
            m_mouseOverCallback = std::move(callback);
            return *this;
        }
        s3d::RectF draw(s3d::StringView text, const s3d::Vec2& pos, const s3d::Optional<s3d::Vec2>& size = s3d::unspecified) const;
    private:
        const s3d::Font& m_font;
        bool m_enabled = true;

        s3d::ColorF m_mouseOverBackColor;
        s3d::ColorF m_textColor;
        s3d::ColorF m_disbaledTextColor;
        s3d::Optional<double> m_fontSize;

        std::function<void()> m_onClickCallback;
        std::function<void()> m_mouseOverCallback;
    };
}