#include <core/Play/PlayStyle/Normal/NormalStyle.hpp>
#include <Useful.hpp>

#include <core/Object/Note/Note.hpp>
#include <core/Object/Bar/Bar.hpp>
#include <core/Object/LongNote/LongNote.hpp>
#include <core/Object/RepeatNote/RepeatNote.hpp>

#include <core/Play/Effects/TapEffect.hpp>
#include <core/Play/Effects/JudgeEffect.hpp>
#include <scenes/utils/Font/FontKinetic.hpp>
#include <scenes/utils/Shaders.hpp>
#include <Siv3D.hpp>

#include <core/Play/PlayMusicGame.hpp>

namespace
{
	using namespace ct;
	using s3d::Math::Pi;

	const Vec2 GetPos(double angle, double count, double scrollRate, double scrollSpeed)
	{
		const double c = Cos(angle);
		const double s = Sin(angle);

		const double r = 40 + count / static_cast<double>(Object::RESOLUTION) * scrollRate * scrollSpeed;
		Vec2 pos;
		pos.x = 400 + r * c;
		pos.y = 300 + r * s;
		return pos;
	}
	//--------------------------------------------------------------------------------
	//関数：GetAngle
	//--------------------------------------------------------------------------------
	//概要：ノーツが来る角度を取得
	//--------------------------------------------------------------------------------
	double GetAngle(NoteType type) {
		static const HashTable<NoteType, double> angleMap
		{
			{ 1,7 * Pi / 6 },
			{ 2, Pi / 2 },
			{ 3,11 * Pi / 6 },
			{ 4, Pi / 6 },
			{ 5,3 * Pi / 2 },
			{ 6,5 * Pi / 6 },
			{ 7,0 },
		};
		if (angleMap.count(type % 10)) {
			return angleMap.at(type % 10);
		}

		return 0.0;
	}
	//--------------------------------------------------------------------------------
	//関数：GetTextureAngle
	//--------------------------------------------------------------------------------
	//概要：ノーツのテクスチャの角度を取得
	//--------------------------------------------------------------------------------
	double GetTextureAngle(NoteType type)
	{
		static const HashTable<NoteType, double> textureAngleMap
		{
			{ 1,-4.0 * Pi / 3.0 },
			{ 2,0.0 },
			{ 3,-2.0 * Pi / 3.0 },
			{ 4,-Pi / 3.0 },
			{ 5,-Pi },
			{ 6,-5.0*Pi / 3.0 },
			{ 7,0.0 },
		};
		if (textureAngleMap.count(type % 10)) {
			return textureAngleMap.at(type % 10);
		}
		return 0.0;
	}

	//--------------------------------------------------------------------------------
	//関数：GetTxetureName
	//--------------------------------------------------------------------------------
	//概要：ノーツのテクスチャの名前を取得
	//--------------------------------------------------------------------------------
	String GetTxetureName(NoteType type)
	{
		static const HashTable<NoteType, String> textureNameMap
		{
			{ 1, U"note_red" },
			{ 2, U"note_blue" },
			{ 3, U"note_yellow" },
			{ 4, U"note_green" },
			{ 5, U"note_orange" },
			{ 6, U"note_purple" },
			{ 7, U"note_black" },
			{ 9, U"note_white" },
			{ 11,U"comet_red" },
			{ 12,U"comet_blue" },
			{ 13,U"comet_yellow" },
			{ 14,U"comet_green" },
			{ 15,U"comet_orange" },
			{ 16,U"comet_purple" },
			{ 17,U"comet_black" },
			{ 18,U"note_rainbow" },
		};
		if (textureNameMap.count(type)) {
			return textureNameMap.at(type);
		}

		return U"note_black";
	}
}

namespace ct
{
	void NormalStyle::drawFrame(bool red, bool blue, bool yellow, std::function<void()> drawCallback) const
	{
		const auto& config = Game::Config();
		auto render = [&] {
			Transformer2D t2d(Mat3x2::Scale(config.m_playScale, Vec2{ 400,300 }));

			if (red)
				TextureAsset(U"center_redlight").drawAt(400, 300);

			if (blue)
				TextureAsset(U"center_bluelight").drawAt(400, 300);

			if (yellow)
				TextureAsset(U"center_yellowlight").drawAt(400, 300);

			TextureAsset(U"center_base").drawAt(400, 300);

			//判定円
			this->drawJudgeLine();

			drawCallback();
		};
		if (config.m_isCirleCut) {
			const Circle mask(400, 300, 300);
			{
				ScopedRenderStates2D blend(BlendState::Subtractive);
				mask.draw(ColorF(0.3, 0.3));
			}
			auto scopedMask = Shaders::Mask().equal([&] {
				mask.draw();
			});
			render();
		} else {
			render();
		}
		{
			ScopedRenderStates2D blend(BlendState::Additive);
			PlayMusicGame::GetEffect(1).update();
		}
	}

	void NormalStyle::drawJudgeLine() const
	{
		static constexpr Color judgeLineColor(255, 165, 0, 255);

		//判定円
		Circle(400, 300, 40).drawFrame(2, 2, judgeLineColor);
	}

	void NormalStyle::drawComboAndRate(int combo, float rate)
	{
		const FontAsset font(FontName::Combo);

		if (combo) {
			const int x = 115;
			TextureAsset(U"combo").draw(x + 121, 299, Palette::White);
			TextureAsset(U"combo").draw(x + 120, 298, Palette::Black);

			String comboText = Pad(combo, { 6, L' ' });
			FontKinetic::DeleteSpace(font, comboText, Vec2{ x, 300 }, Palette::Black, Palette::White);
		}

		font(U"{:.2f}%"_fmt(rate)).draw(501, 301, Palette::White);
		font(U"{:.2f}%"_fmt(rate)).draw(500, 300, Palette::Black);
	}

	void NormalStyle::drawTapEffect(int type)
	{
		if (type == 9) {
			PlayMusicGame::GetEffect().add<TapEffect>(0, 9);
			PlayMusicGame::GetEffect(1).add<TapEffect2_2>(GetPos(0, 0, 0, 0));
			PlayMusicGame::GetEffect().add<TapEffect>(Pi, 9);
			PlayMusicGame::GetEffect(1).add<TapEffect2_2>(GetPos(Pi, 0, 0, 0));
		} else if (type == 7 || type == 17) {
			PlayMusicGame::GetEffect().add<TapEffect>(1 * Pi / 6, 7);
			PlayMusicGame::GetEffect(1).add<TapEffect2_2>(GetPos(1 * Pi / 6, 0, 0, 0));
			PlayMusicGame::GetEffect().add<TapEffect>(9 * Pi / 6, 7);
			PlayMusicGame::GetEffect(1).add<TapEffect2_2>(GetPos(9 * Pi / 6, 0, 0, 0));
			PlayMusicGame::GetEffect().add<TapEffect>(5 * Pi / 6, 7);
			PlayMusicGame::GetEffect(1).add<TapEffect2_2>(GetPos(5 * Pi / 6, 0, 0, 0));
		} else if (type == 18 || type == 10) {
			PlayMusicGame::GetEffect().add<TapEffect>(3 * Pi / 6, 7);
			PlayMusicGame::GetEffect(1).add<TapEffect2_2>(GetPos(3 * Pi / 6, 0, 0, 0));
			PlayMusicGame::GetEffect().add<TapEffect>(7 * Pi / 6, 7);
			PlayMusicGame::GetEffect(1).add<TapEffect2_2>(GetPos(7 * Pi / 6, 0, 0, 0));
			PlayMusicGame::GetEffect().add<TapEffect>(11 * Pi / 6, 7);
			PlayMusicGame::GetEffect(1).add<TapEffect2_2>(GetPos(1 * Pi / 6, 0, 0, 0));
		} else {
			PlayMusicGame::GetEffect().add<TapEffect>(GetAngle(type), type % 10);
			PlayMusicGame::GetEffect(1).add<TapEffect2_2>(GetPos(GetAngle(type), 0, 0, 0));
		}
	}

	void NormalStyle::drawJudgeEffect(const String& str, int type)
	{
		if (type == 9) {
			PlayMusicGame::GetEffect().add<JudgeEffect>(str, GetPos(3 * Pi / 2, 2400, 1.0f, 1.0));
		} else if (type == 7 || type == 17 || type == 10) {
			PlayMusicGame::GetEffect().add<JudgeEffect>(str, GetPos(3 * Pi / 2, 2400, 1.0f, 1.0));
		} else {
			PlayMusicGame::GetEffect().add<JudgeEffect>(str, GetPos(GetAngle(type), 2400, 1.0f, 1.0));
		}

	}

	void NormalStyle::draw(const Bar& note, double count, double scrollRate) const
	{
		double x = 40 + count / static_cast<double>(Object::RESOLUTION) * scrollRate * note.getSpeed();

		if (x <= 0 || x > 1000)
			return;
		Circle(400, 300, x).drawFrame(2, 2, ColorF(0, 0.2));
	}

	void NormalStyle::draw(const Note& note, double count, double scrollRate)const
	{
		const NoteType type = note.getType();
		const String& textureName = GetTxetureName(type);
		const double angle = GetAngle(type);
		const double& speed = note.getSpeed();

		if (type >= 11 && note.isFirstTap())
			count = 0;

		const auto pos = ::GetPos(angle, count, scrollRate, speed);

		if (!Note::CanDraw(pos))
			return;
		const auto& texture = TextureAsset(textureName);
		if (type == 7 || type == 17) {
			texture.rotated(-Pi / 3.0).drawAt(::GetPos((1 + (count * scrollRate) / 10000.0) * Pi / 6, count, scrollRate, speed));
			texture.rotated(-5.0 * Pi / 3.0).drawAt(::GetPos((5 + (count * scrollRate) / 10000.0) * Pi / 6, count, scrollRate, speed));
			texture.rotated(-Pi).drawAt(::GetPos((9 + (count * scrollRate) / 10000.0) * Pi / 6, count, scrollRate, speed));
			return;
		} else if (type == 18) {
			texture.drawAt(GetPos((3 + (-count * scrollRate) / 10000.0) * Pi / 6, count, scrollRate, speed));
			texture.rotated(-4.0 * Pi / 3.0).drawAt(GetPos((7 + (-count * scrollRate) / 10000.0) * Pi / 6, count, scrollRate, speed));
			texture.rotated(-2.0 * Pi / 3.0).drawAt(GetPos((11 + (-count * scrollRate) / 10000.0) * Pi / 6, count, scrollRate, speed));
			return;
		} else if (type == 9) {
			TextureAsset(U"note_white").drawAt(::GetPos(Pi, count, scrollRate, speed));
		}

		double textureAngle = ::GetTextureAngle(type);
		texture.rotated(textureAngle).drawAt(pos);
	}

	namespace
	{
		template<class ColorType>
		void DrawLongTail(double count, double pCount, double speed, double pSpeed, double scrollRate, double offset, const Texture& texture, const ColorType& color, bool isReturn = false)
		{
			const double scroll = isReturn ? -scrollRate : scrollRate;
			const double rotate = isReturn ? Pi * (-(15 - offset) / 6.0)
				: Pi * (-(9 - offset) / 6.0);
			const Vec2 pos = ::GetPos((offset + (pCount * scroll) / 10000.0) * Pi / 6, count, scrollRate, speed);
			const Vec2 pPos = ::GetPos((offset + (pCount * scroll) / 10000.0) * Pi / 6, pCount, scrollRate, pSpeed);
			if constexpr (std::is_array_v<ColorType>) {
				Line(pos, pPos)
					.draw(8, ColorF(0, 0.5))
					.draw(4, color[0], color[1])
					;
			} else {
				Line(pos, pPos)
					.draw(8, ColorF(0, 0.5))
					.draw(4, color)
					;
			}
			texture.rotated(rotate).drawAt(pos);
		}
	}
	void NormalStyle::draw(const LongNote& note, double count, double scrollRate) const
	{
		const auto parent = note.getParent();
		const auto angle = ::GetAngle(parent->getType());
		const auto& speed = note.getSpeed();
		const NoteType type = note.getType();
		const String& textureName = GetTxetureName(type);
		const Color& color = note.getColor();
		Vec2 pos = ::GetPos(angle, count, scrollRate, speed);

		const double nowCount = note.getDrawCount() - count;
		auto pCount = parent->getDrawCount() - nowCount;
		if (parent->isFirstTap())
			pCount = 0;
		auto pPos = ::GetPos(angle, pCount, scrollRate, parent->getSpeed());

		if (!Note::CanDraw(pPos) && !Note::CanDraw(pos))
			return;

		if (type == 17) {
			const TextureAsset texture(textureName);
			::DrawLongTail(count, pCount, speed, parent->getSpeed(), scrollRate, 1, texture, color);
			::DrawLongTail(count, pCount, speed, parent->getSpeed(), scrollRate, 5, texture, color);
			::DrawLongTail(count, pCount, speed, parent->getSpeed(), scrollRate, 9, texture, color);
			return;
		}
		Line(pos, pPos).draw(8, ColorF(0, 0.5)).draw(4, color);

		double textureAngle = ::GetTextureAngle(type) - Pi;
		TextureAsset(textureName).rotated(textureAngle).drawAt(pos);
	}

	void NormalStyle::draw(const RepeatNote& note, double count, double scrollRate) const
	{
		if (note.isFirstTap())
			count = 0;

		auto speed = note.getSpeed();
		const auto pos = GetPos(0, count, scrollRate, speed);

		if (!Note::CanDraw(pos))
			return;

		{
			const TextureAsset texture(U"comet_rainbow_head");
			texture.drawAt(GetPos((3 + (-count * scrollRate) / 10000.0) * Pi / 6, count, scrollRate, speed));
			texture.rotated(-4.0 * Pi / 3.0).drawAt(GetPos((7 + (-count * scrollRate) / 10000.0) * Pi / 6, count, scrollRate, speed));
			texture.rotated(-2.0 * Pi / 3.0).drawAt(GetPos((11 + (-count * scrollRate) / 10000.0) * Pi / 6, count, scrollRate, speed));
		}
	}

	void NormalStyle::draw(const RepeatEnd& note, double count, double scrollRate) const
	{
		const auto parent = note.getParent();
		const auto angle = ::GetAngle(parent->getType());
		const auto speed = note.getSpeed();
		const Vec2 pos = ::GetPos(angle, count, scrollRate, speed);

		const double nowCount = note.getDrawCount() - count;
		auto pCount = parent->getDrawCount() - nowCount;
		if (parent->isFirstTap())
			pCount = 0;
		auto pPos = GetPos(angle, pCount, scrollRate, parent->getSpeed());

		if (!Note::CanDraw(pPos) && !Note::CanDraw(pos))
			return;

		{
			Color c1 = HSV(static_cast<int>(count / 10) % 360, 0.5, 1);
			Color c2 = HSV((static_cast<int>(count / 10) + 72) % 360, 0.5, 1);

			const TextureAsset texture(U"comet_rainbow_tail");
			const Color(&color)[2] = { c1,c2 };
			::DrawLongTail(count, pCount, speed, parent->getSpeed(), scrollRate, 3, texture, color, true);
			::DrawLongTail(count, pCount, speed, parent->getSpeed(), scrollRate, 7, texture, color, true);
			::DrawLongTail(count, pCount, speed, parent->getSpeed(), scrollRate, 11, texture, color, true);
		}
	}

}
