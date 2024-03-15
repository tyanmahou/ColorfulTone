#include <scenes/utils/SharedDraw.hpp>
#include <Useful.hpp>
#include <core/Play/HighSpeed/HighSpeedDemo.hpp>
#include <core/Play/PlayNotesData.hpp>
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
		String tmp;
		if (music.getMaxBPM() == music.getMinBPM()) {
			double result = music.getMinBPM() * scrollRate;
			tmp = U"{:.0f}*{:.1f}={:.1f}"_fmt(music.getMinBPM(), scrollRate, result);
		} else {
			double resultMin = music.getMinBPM() * scrollRate;
			double resultMax = music.getMaxBPM() * scrollRate;
			tmp = U"{:.0f}-{:.0f}*{:.1f}={:.1f}-{:.1f}"_fmt(music.getMinBPM(), music.getMaxBPM(), scrollRate, resultMin, resultMax);
		}
		const Vec2 topLeft{ 25 + 10, 533 };
		FontAsset(FontName::Bpm)(tmp).draw(topLeft + Vec2{ 1,1 }, Palette::Black);
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
	void HighSpeedPlay(const HighSpeedDemo& highSpeedDemo, const PlayNotesData& noteData, double scrollRate, bool canDemo)
	{
		const MusicData music = noteData.getMusic();
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
				colorBack = Palette::Black;
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
	void DrawPlayContextHeader(const s3d::Vec2& pos)
	{
		if (PlayContext::IsAutoPlay() && PlayContext::IsPracticePlay()) {
			PutText(U"AutoPlay / Practice", Arg::center = pos);
		} else if (PlayContext::IsAutoPlay()) {
			PutText(U"AutoPlay", Arg::center = pos);
		} else if (PlayContext::IsPracticePlay()) {
			PutText(U"Practice", Arg::center = pos);
		}
	}
	void DrawRandomNoteType()
	{
		DrawRandomNoteType(Vec2{ 800 - 10, Scene::Height() - 55 });
	}
	void DrawRandomNoteType(const s3d::Vec2& pos)
	{
		if (Game::Config().m_random != RandomNoteType::None) {
			const FontAsset font(FontName::Bpm);
			StringView text = ToRandomNoteTypeStr(Game::Config().m_random);
			font(text).draw(Arg::rightCenter = pos + Vec2{1,1}, Palette::Black);
			font(text).draw(Arg::rightCenter = pos, Palette::White);
		}
	}
	void DrawPlayStyleIcon()
	{
		DrawPlayStyleIcon({ 5, 533 });
	}
	void DrawPlayStyleIcon(const s3d::Vec2& pos)
	{
		TextureAsset(U"playstyle_icon")(static_cast<int32>(Game::Config().m_styleType) * 50, 0, 50, 50).resized(25, 25).draw(pos, Palette::Orange);
	}
}
