#pragma once
#include <Scenes/utils/SceneInfo.hpp>
#include <Scenes/utils/ScopedResetTransRT.hpp>

#include <Siv3D/Font.hpp>

namespace ct
{
    void ContractionDrawbleString(const s3d::DrawableText& font, const s3d::Vec2& pos, double width, const s3d::Color& color = s3d::Palette::White, bool drawCenter = true);
    void ContractionDrawbleString(const s3d::TextStyle& style, const s3d::DrawableText& font, const s3d::Vec2& pos, double width, const s3d::Color& color = s3d::Palette::White, bool drawCenter = true);
    void ContractionDrawbleStringOutline(const s3d::DrawableText& font, const s3d::Vec2& pos, double width, const s3d::Color& color = s3d::Palette::White, bool drawCenter = true);
}