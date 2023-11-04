#include <scenes/utils/Util.hpp>
#include <Siv3D.hpp>

namespace ct
{
    void ContractionDrawbleString(const s3d::DrawableText& font, const s3d::Vec2& pos, int width, const s3d::Color& color, bool drawCenter)
    {
		const auto nameWidth = font.region().w;
		const auto getMat = [&]() {
			if (nameWidth > width) {
				const auto scale = static_cast<double>(width) / nameWidth;
				return Mat3x2::Scale(scale, 1, pos);
			}
			return Mat3x2::Identity();
			};
		Transformer2D t2d(getMat());

		//曲名
		if (drawCenter) {
			font.drawAt(pos, color);
		} else {
			font.draw(pos, color);
		}
    }
}
