#pragma once
#include <Siv3D.hpp>
namespace ct
{
    struct BGLight : IEffect
    {
        double m_maxRadius;
        double m_speed;
        double m_deflection;
        int m_h;
        Vec2 m_pos;

        BGLight() :
            m_maxRadius(Random(10.0, 150.0)),
            m_speed(Random(-5.0, -1.0)),
            m_deflection(Random(0.0, 100.0)),
            m_pos(RandomVec2(Rect{ 0,300,800,600 })),
            m_h(static_cast<s3d::int32>(Scene::Time() * 60) % 360)
        {}

        bool update(double t) override
        {
            constexpr Color color{ 250,255,210,100 };
            const double r = Min(m_maxRadius, EaseOut(Easing::Cubic, 0.0, m_maxRadius, t));
            m_pos.y += m_speed * Scene::DeltaTime() * 60.0;
            Vec2 pos = m_pos + Vec2{ m_deflection * Sin(t),0.0 };
            HSV hsv = HSV(color);
            hsv.h = m_h;
            Circle(pos, r).draw(hsv.toColor(t > 8.0 ? static_cast<uint32>(100 - (t - 8.0) * 50) : 100));
            return m_pos.y + r >= 0;
        }
    };
}
