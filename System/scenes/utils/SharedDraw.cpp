#include <scenes/utils/SharedDraw.hpp>
#include <scenes/utils/Util.hpp>
#include <commons/FontName.hpp>
#include <Siv3D.hpp>

namespace ct::SharedDraw
{
    void Sticky(const String* blueText, const String* redText)
    {
		const auto& font = FontAsset(FontName::StickyLabel);
		// red
		{
			Transformer2D t2d(Mat3x2::Rotate(Math::ToRadians(-15)).translated({ 110, 50 }));
			TextureAsset(U"sticky_red").draw();
			if (redText) {
				ContractionDrawbleString(
					font(*redText),
					{ 125,25 },
					175,
					Palette::Black
				);
			}
		}
		// blue
		{
			Transformer2D t2d(Mat3x2::Rotate(Math::ToRadians(-20)).translated({ -25, 60 }));
			TextureAsset(U"sticky").draw();
			if (blueText) {
				ContractionDrawbleString(
					font(*blueText),
					{ 125,25 },
					175,
					Palette::Black
				);
			}
		}
    }
}
