#include <scenes/utils/Util.hpp>
#include <Siv3D.hpp>

namespace
{
	Mat3x2 GetMat(const s3d::DrawableText& font, const s3d::Vec2& pos, double width)
	{
		const auto nameWidth = font.region().w;
		if (nameWidth > width) {
			const auto scale = width / nameWidth;
			return Mat3x2::Scale(scale, 1, pos);
		}
		return Mat3x2::Identity();
	}
}
namespace ct
{
	void ContractionDrawbleStringBR(const s3d::DrawableText& font, const s3d::Vec2& pos, double width, const s3d::Color& color)
	{
		Transformer2D t2d{ GetMat(font, pos, width) };
		font.draw(Arg::bottomRight = pos, color);
	}
	void ContractionDrawbleString(const s3d::DrawableText& font, const s3d::Vec2& pos, double width, const s3d::Color& color, bool drawCenter)
    {
		Transformer2D t2d{ GetMat(font, pos, width) };

		//曲名
		if (drawCenter) {
			font.drawAt(pos, color);
		} else {
			font.draw(pos, color);
		}
    }
	void ContractionDrawbleString(const s3d::TextStyle& style, const s3d::DrawableText& font, const s3d::Vec2& pos, double width, const s3d::Color& color, bool drawCenter)
	{
		Transformer2D t2d{ GetMat(font, pos, width) };

		//曲名
		if (drawCenter) {
			font.drawAt(style, pos, color);
		} else {
			font.draw(style, pos, color);
		}
	}

	void ContractionDrawbleStringOutline(const s3d::DrawableText& font, const s3d::Vec2& pos, double width, const s3d::Color& color, bool drawCenter)
	{
		ContractionDrawbleString(TextStyle::Outline(0.3, color), font, pos, width, color, drawCenter);
	}
}
