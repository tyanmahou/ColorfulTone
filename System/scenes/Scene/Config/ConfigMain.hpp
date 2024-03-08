#pragma once
#include <scenes/Scene/Config/Config.hpp>

namespace ct
{
    class ConfigMain
    {
    public:
        ConfigMain();

        bool update();
        void draw()const;
        void drawWithBack(const s3d::ColorF& color = s3d::ColorF(0, 0.8))const;
        void setActive(bool isActive)
        {
            m_isActive = isActive;
        }
        bool isActive()const
        {
            return m_isActive;
        }
        void reset();
        bool isRoot()const;
    private:
        ConfigManager m_config;
        bool m_isActive = true;
    };
}
