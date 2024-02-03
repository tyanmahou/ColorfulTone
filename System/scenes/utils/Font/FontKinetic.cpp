#include <scenes/utils/Font/FontKinetic.hpp>
#include <Siv3D.hpp>

namespace ct
{
	void FontKinetic::DeleteSpace(const s3d::Font& font, s3d::StringView text, const Vec2& topLeft, const s3d::Color& color)
	{
		Vec2 penPos{ topLeft };
		for (const Glyph& glyph : font.getGlyphs(text)) {
			// 改行文字なら
			if (glyph.codePoint == U'\n') {
				// ペンの X 座標をリセット
				penPos.x = topLeft.x;

				// ペンの Y 座標をフォントの高さ分進める
				penPos.y += font.height();

				continue;
			}

			if (glyph.codePoint != U' ') {
				glyph.texture.draw(Math::Round(penPos + glyph.getOffset()), color);
			}

			penPos.x += glyph.xAdvance;
		}
	}
	void FontKinetic::DeleteSpace(const s3d::Font& font, s3d::StringView text, const Vec2& topLeft, const s3d::Color& color, const s3d::Color& shadeColor)
	{
		FontKinetic::DeleteSpace(font, text, topLeft + Vec2{ 1, 1 }, shadeColor);
		FontKinetic::DeleteSpace(font, text, topLeft, color);
	}
	void FontKinetic::DeleteSpace(const s3d::Font& font, s3d::StringView text, s3d::TextStyle& style, const Vec2& topLeft, const s3d::Color& color)
	{
		// MSDF フォント + 影 + 輪郭描画のための設定
		const ScopedCustomShader2D shader{ Font::GetPixelShader(font.method(), style.type) };
		Graphics2D::SetSDFParameters(style);

		DeleteSpace(font, text, topLeft, color);
	}
	void FontKinetic::LineSpacing(const s3d::Font& font, s3d::StringView text, const Vec2& topLeft, const s3d::Color& color, double lineSpacing)
	{
		Vec2 penPos{ topLeft };
		for (const Glyph& glyph : font.getGlyphs(text)) {
			// 改行文字なら
			if (glyph.codePoint == U'\n') {
				// ペンの X 座標をリセット
				penPos.x = topLeft.x;

				// ペンの Y 座標をフォントの高さ分進める
				penPos.y += font.height() * lineSpacing;

				continue;
			}
			glyph.texture.draw(Math::Round(penPos + glyph.getOffset()), color);
			penPos.x += glyph.xAdvance;
		}
	}
}