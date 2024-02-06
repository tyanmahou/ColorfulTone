#include <scenes/utils/SharedDraw/LoadingCircle.hpp>
#include <Siv3D.hpp>

namespace ct::SharedDraw
{
    const s3d::Circle& LoadingCircle::DrawMain(const s3d::ColorF& color, double t)
    {
        return DrawMain(Scene::CenterF(), 200, color, t);
    }
    const s3d::Circle& LoadingCircle::DrawMain(const s3d::Vec2& center, double radius, const s3d::ColorF& color, double t)
    {
        return Draw(center, radius, 2s, color, t);
    }
    const s3d::Circle& LoadingCircle::Draw(double radius, const s3d::Duration& period, const s3d::ColorF& color, double t)
    {
        return Draw(Scene::CenterF(), radius, period, color, t);
    }
    const s3d::Circle& LoadingCircle::Draw(const s3d::Vec2& center, double radius, const s3d::Duration& period, const s3d::ColorF& color, double t)
    {
        return Draw(Circle{ center, radius }, period, 0, 3, color, t);
    }
    const s3d::Circle& LoadingCircle::Draw(const s3d::Circle& circle, const s3d::Duration& period, double innerThickness, double outerThickness, const ColorF& color, double t)
    {
        const double rotateRate = s3d::Periodic::Sawtooth0_1(period * 0.75, t);
        const double offsetAngle = rotateRate * Math::TwoPi;

        const double rate0_2 = s3d::Periodic::Sawtooth0_1(period, t) * 2.0;
        double startAngle = offsetAngle;
        if (rate0_2 <= 1) {
            startAngle += Math::Lerp(0, Math::TwoPi, rate0_2);
        } else {
            startAngle += Math::TwoPi;
        }
        double endAngle = offsetAngle;
        if (rate0_2 <= 1) {
            endAngle += Math::Lerp(0, Math::HalfPi, rate0_2);
        } else {
            endAngle += Math::Lerp(Math::HalfPi, Math::TwoPi, rate0_2 - 1);
        }
        const double angle = -(startAngle - endAngle);
        return circle.drawArc(startAngle, angle, innerThickness, outerThickness, color);
    }
}
