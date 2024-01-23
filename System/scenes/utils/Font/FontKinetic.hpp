#pragma once

namespace ct
{
	class FontKinetic
	{
	public:
		static void DeleteSpace(const s3d::Font& font, s3d::StringView text, const Vec2& topLeft, const s3d::Color& color);
		static void DeleteSpace(const s3d::Font& font, s3d::StringView text, const Vec2& topLeft, const s3d::Color& color, const s3d::Color& shadeColor);
		static void DeleteSpace(const s3d::Font& font, s3d::StringView text, s3d::TextStyle& style, const Vec2& topLeft, const s3d::Color& color);
	};
}