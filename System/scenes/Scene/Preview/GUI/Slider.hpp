#pragma once
#include <Siv3D/RectF.hpp>

namespace ct::GUI
{
    class Slider
    {
    public:
        Slider(double& value);
        Slider& setEnabled(bool enabled)
        {
            m_enabled = enabled;
            return *this;
        }
        Slider& setWidth(double width)
        {
            m_width = width;
            return *this;
        }
        Slider& setHeight(double height)
        {
            m_height = height;
            return *this;
        }
        Slider& setBaseColor(const s3d::ColorF& color)
        {
            m_baseColor = color;
            return *this;
        }
        Slider& setOnChangeValue(std::function<void(double)>&& callback)
        {
            m_onChangeValue = std::move(callback);
            return *this;
        }
        s3d::RectF draw(const s3d::Vec2& posTL) const;
    private:
        double& m_value;

        bool m_enabled = true;
        double m_width = 100;
        double m_height = 30;

        s3d::ColorF m_baseColor;

        std::function<void(double)> m_onChangeValue;
    };
}