#pragma once
#include"Config.hpp"

class ConfigMain
{
    ConfigManager m_config;
    bool m_isActive = true;
public:
    ConfigMain();

    bool update();
    void draw()const;

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
};