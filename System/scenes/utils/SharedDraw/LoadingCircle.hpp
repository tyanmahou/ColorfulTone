#pragma once
#include <Siv3D/Circle.hpp>
#include <Siv3D/Duration.hpp>
#include <Siv3D/Scene.hpp>

namespace ct::SharedDraw
{
    class LoadingCircle
    {
    public:
        static const s3d::Circle& DrawMain(const s3d::ColorF& color = s3d::ColorF(0, 0.5), double t = s3d::Scene::Time());
        static const s3d::Circle& DrawMain(const s3d::Vec2& center, double radius, const s3d::ColorF& color = s3d::ColorF(0, 0.5), double t = s3d::Scene::Time());

        static const s3d::Circle& Draw(double radius, const s3d::Duration& period, const s3d::ColorF& color = s3d::Palette::White, double t = s3d::Scene::Time());
        static const s3d::Circle& Draw(const s3d::Vec2& center, double radius, const s3d::Duration& period, const s3d::ColorF& color = s3d::Palette::White, double t = s3d::Scene::Time());
        static const s3d::Circle& Draw(const s3d::Circle& circledouble, const s3d::Duration& period, double innerThickness = 0.0, double outerThickness = 3.0, const s3d::ColorF& color = s3d::Palette::White, double t = s3d::Scene::Time());
    };
}