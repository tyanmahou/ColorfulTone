#include "SharedDraw.hpp"
#include "BGLight.hpp"
#include "HighSpeedDemo.h"
#include "MusicData.h"
#include "CourseData.h"
namespace SharedDraw
{
	void DrawBGLight::update()
	{
		if (System::FrameCount() % 20 == 0)
		{
			m_effect.add<BGLight>();
		}
	}

	void DrawBGLight::draw() const
	{
		m_effect.update();
	}

	void Sticky(const String * blueText, const String * redText)
	{
		const auto& font = FontAsset(L"bpm");
		// red
		{
			util::Transformer2D t2d(Mat3x2::Rotate(Math::Radians(-15)).translate({ 110, 50 }));
			TextureAsset(L"sticky_red").draw();
			if (redText)
			{
				util::ContractionDrawbleString(
					font(*redText),
					{ 125,25 },
					175,
					Palette::Black
				);
			}
		}
		// blue
		{
			util::Transformer2D t2d(Mat3x2::Rotate(Math::Radians(-20)).translate({ -25, 60 }));
			TextureAsset(L"sticky").draw();
			if (blueText)
			{
				util::ContractionDrawbleString(
					font(*blueText),
					{ 125,25 },
					175,
					Palette::Black
				);
			}
		}
	}

	JacketInfo::JacketInfo() :
		m_font12(FontAsset(L"bpm")),
		m_font16b(FontAsset(L"selectMusics")),
		m_pos(Constants::JacketCenter, 475)
	{}

	JacketInfo & JacketInfo::setPos(const Vec2 & pos)
	{
		m_pos = pos;
		return *this;
	}

	const JacketInfo & JacketInfo::drawLabel(const Texture & tex, double t)const
	{
		tex.uv(0, 100.0 / 800.0, t, 30.0 / 800.0).resize(800 * t, 30).draw();
		tex.uv(1 - t, 630.0 / 800.0, t, 30.0 / 800.0).resize(800 * t, 30).draw(800 - 800 * t, 530);
		return *this;
	}

	const JacketInfo & JacketInfo::drawLabel()const
	{
		Rect(0, 0, 800, 30).draw(Color(0, 0, 0, 100));
		Rect(0, 530, 800, 30).draw(Color(0, 0, 0, 100));
		return *this;
	}

	const JacketInfo& JacketInfo::drawLine() const
	{
		TextureAsset(L"line").drawAt(m_pos);
		return *this;
	}

	const JacketInfo& JacketInfo::drawTitle(const String & title, const Color& color) const
	{
		util::ContractionDrawbleString(
			m_font16b(title),
			m_pos + Vec2{ 0,-35 },
			Constants::JacketWidth,
			color
		);
		return *this;
	}

	const JacketInfo & JacketInfo::drawSub(const String & sub, const Color& color) const
	{
		util::ContractionDrawbleString(
			m_font12(sub),
			m_pos,
			Constants::JacketWidth,
			color
		);
		return *this;
	}

	const JacketInfo & JacketInfo::drawDetail(const String & detail, const Color& color) const
	{
		util::ContractionDrawbleString(
			m_font12(detail),
			m_pos + Vec2{ 0, 35 },
			Constants::JacketWidth,
			color
		);
		return *this;
	}
	const JacketInfo & JacketInfo::drawDetailRight(const String & detail, const Color & color) const
	{
		const int wSize = 12;
		const int width = wSize * detail.length;

		const auto kinetic = [wSize](KineticTypography& k)
		{
			k.pos.x = k.origin.x + wSize * k.index;
		};
		m_font12(detail).drawKinetic(
			m_pos + Vec2{ Constants::JacketWidth / 2.0 - width, 20 },
			kinetic,
			color
		);
		return *this;
	}
	void HighSpeed(const HighSpeedDemo& highSpeedDemo, const MusicData& music, float scrollRate)
	{
		String tmp = Format(music.getBPM(), L"*", scrollRate);

		const auto kineticFunction = [=](KineticTypography& k)
		{
			static int fBpm = 0;
			static int eBpm = 0;

			if (k.ch == '*')
				fBpm = k.index;
			if (k.ch == '=')
				eBpm = k.index;
			if (Input::KeyControl.pressed)
				if (k.index > fBpm&&k.index < eBpm)
					k.col = Palette::Red;

		};

		if (const auto result = EvaluateOpt(tmp))
		{
			FontAsset(L"bpm")(tmp, L"=", result.value()).drawKinetic(10, 533, kineticFunction);
		}
		if (Input::KeyControl.pressed)
		{
			highSpeedDemo.draw(music.getMinSoundBeat(), music.getMaxSoundBeat(), scrollRate);
		}
	}
	MemoInfo::MemoInfo():
		m_pos(DefaultPos)
	{}
	MemoInfo & MemoInfo::setPos(const Vec2 & pos)
	{
		m_pos = pos;
		return *this;
	}
	void MemoInfo::draw(const NotesData & notes) const
	{
		util::Transformer2D t2d(Mat3x2::Rotate(Math::Radians(8)).translate(m_pos));
		// フォント
		const auto& font12 = FontAsset(L"bpm");
		Graphics2D::SetSamplerState(SamplerState::ClampLinear);
		TextureAsset(L"memo").drawAt({0, 0});
		Graphics2D::SetSamplerState(SamplerState::Default2D);

		const ScoreModel& score = notes.getScore();
		// クリアレート
		constexpr Vec2 ratePos{-55,-63};
		FontAsset(L"level")(L"{:.2f}%"_fmt, score.clearRate).drawCenter(ratePos, Palette::Black);
		// 譜面製作者
		constexpr Vec2 designerPos = ratePos + Vec2{ 0, 60 };
		util::ContractionDrawbleString(
			font12(notes.getNotesArtistName()),
			designerPos,
			188,
			Palette::Black
		);
		// 総合ノーツ数
		constexpr Vec2 totalPos = designerPos + Vec2{ 0, 60 };
		font12(notes.getTotalNotes()).drawCenter(totalPos, Palette::Black);
		// クリア情報
		constexpr Vec2 clearIconPos{ 90, -48 };
		constexpr Vec2 fcIconPos = clearIconPos + Vec2{ 0, 60 };
		if (score.isClear)
		{
			TextureAsset(L"iconClear").scale(0.5).drawAt(clearIconPos);
		}
		if (score.specialResult == SpecialResult::All_Perfect)
		{
			TextureAsset(L"iconAP").scale(0.5).drawAt(fcIconPos);
		}
		else if (score.specialResult == SpecialResult::Full_Combo)
		{
			TextureAsset(L"iconFC").scale(0.5).drawAt(fcIconPos);
		}
	}
	void MemoInfo::draw(const CourseScore & courseScore) const
	{
		util::Transformer2D t2d(Mat3x2::Rotate(Math::Radians(8)).translate(m_pos));

		Graphics2D::SetSamplerState(SamplerState::ClampLinear);
		TextureAsset(L"memo_course").drawAt({ 0, 0 });
		Graphics2D::SetSamplerState(SamplerState::Default2D);

		if (courseScore.isClear)
		{
			TextureAsset(L"pass").scale(0.3).drawAt({ 230, 60});
		}
		else if(courseScore.totalRate  > 0)
		{
			TextureAsset(L"noPass").scale(0.3).drawAt({ 230, 60 });
		}
		// クリアレート
		constexpr Vec2 ratePos{ -55,-63 };
		FontAsset(L"level")(L"{:.2f}%"_fmt, courseScore.totalRate).drawCenter(ratePos, Palette::Black);
		// ライフ
		constexpr Vec2 lifePos = ratePos + Vec2{ 0, 60 };
		FontAsset(L"bpm")(L"{:.2f}%"_fmt, courseScore.life).drawCenter(lifePos, Palette::Black);
	}
}