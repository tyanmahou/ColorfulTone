#include <scenes/utils/SharedDraw.hpp>
#include <scenes/utils/Util.hpp>
#include <core/Play/HighSpeed/HighSpeedDemo.hpp>
#include <core/Data/MusicData/MusicData.hpp>
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
	void HighSpeed(const HighSpeedDemo& highSpeedDemo, const MusicData& music, double scrollRate, bool canDemo)
	{
		double result = music.getBPM() * scrollRate;
		String tmp = U"{}*{:.1f}={:.1f}"_fmt(music.getBPM(),  scrollRate, result);

		const Vec2 topLeft{ 10, 533 };
		Vec2 penPos{ topLeft };
		for (const auto& [i, glyph] : s3d::Indexed(FontAsset(FontName::Bpm).getGlyphs(tmp))) {
			static size_t fBpm = 0;
			static size_t eBpm = 0;

			if (glyph.codePoint == U'*') {
				fBpm = i;
			}
			if (glyph.codePoint == U'=') {
				eBpm = i;
			}
			ColorF color = Palette::White;
			if (KeyControl.pressed() && i > fBpm && i < eBpm) {
				color = Palette::Red;
			}
			Vec2 drawPos = penPos + glyph.getOffset();
			glyph.texture.draw(Math::Round(drawPos), color);

			penPos.x += glyph.xAdvance;
		}
		if (canDemo && KeyControl.pressed()) {
			highSpeedDemo.draw(music.getMinSoundBeat(), music.getMaxSoundBeat(), scrollRate);
		}
	}
	void HighSpeedPlay(const HighSpeedDemo& highSpeedDemo, const NotesData& noteData, double scrollRate, bool canDemo)
	{
		const MusicData music = *noteData.getMusic();
		double result = noteData.getCurrentBPM() * scrollRate;
		String tmp = U"{}*{:.1f}={:.1f}"_fmt(static_cast<int32>(noteData.getCurrentBPM()), scrollRate, result);

		const Vec2 topLeft{ 10, 533 };
		Vec2 penPos{ topLeft };
		for (const auto& [i, glyph] : s3d::Indexed(FontAsset(FontName::Bpm).getGlyphs(tmp))) {
			static size_t fBpm = 0;
			static size_t eBpm = 0;

			if (glyph.codePoint == U'*') {
				fBpm = i;
			}
			if (glyph.codePoint == U'=') {
				eBpm = i;
			}
			ColorF color = Palette::White;
			ColorF colorBack = Palette::Black;
			if (KeyControl.pressed() && i > fBpm && i < eBpm) {
				color = Palette::Red;
				colorBack = Palette::White;
			}
			Vec2 drawPos = penPos + glyph.getOffset();
			glyph.texture.draw(Math::Round(drawPos) + Vec2{ 1, 1 }, colorBack);
			glyph.texture.draw(Math::Round(drawPos), color);

			penPos.x += glyph.xAdvance;
		}
		if (canDemo && KeyControl.pressed()) {
			highSpeedDemo.draw(music.getMinSoundBeat(), music.getMaxSoundBeat(), scrollRate);
		}
	}
	void LongPressBack(const s3d::InputGroup& input, const s3d::String& text)
	{
		if (!input.pressed()) {
			return;
		}
		const double backAlpha = 0.3 + Min(input.pressedDuration().count(), 0.7);
		ColorF color{ 0, backAlpha };
		FontAsset(FontName::Regular20)(text).drawAt(400, 400, color);
	}
}
