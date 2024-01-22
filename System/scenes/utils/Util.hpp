#pragma once
#include <Scenes/utils/SceneInfo.hpp>
#include <Scenes/utils/ScopedResetTransRT.hpp>

#include <Siv3D/Font.hpp>

namespace ct
{
    void ContractionDrawbleString(const s3d::DrawableText& font, const s3d::Vec2& pos, int width, const s3d::Color& color = s3d::Palette::White, bool drawCenter = true);
}