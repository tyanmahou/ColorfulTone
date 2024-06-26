﻿#include <scenes/utils/SharedDraw/MemoInfo.hpp>
#include <scenes/utils/Util.hpp>
#include <core/Data/NotesData/NotesData.hpp>
#include <core/Data/CourseData/CourseData.hpp>
#include <core/Play/LifeGauge/LifeGauge.hpp>
#include <commons/FontName.hpp>
#include <Siv3D.hpp>

namespace
{
	constexpr TextStyle style = TextStyle::Outline(0.3, Palette::Black);
}
namespace ct::SharedDraw
{

    MemoInfo::MemoInfo() :
        m_pos(DefaultPos)
    {}
    MemoInfo& MemoInfo::setPos(const Vec2& pos)
    {
        m_pos = pos;
        return *this;
    }
    MemoInfo& MemoInfo::setPosFromDefault(const s3d::Vec2& pos)
    {
        return this->setPos(DefaultPos + pos);
    }

	void MemoInfo::draw(const NotesData& notes) const
	{
		Transformer2D t2d(Mat3x2::Rotate(Math::ToRadians(8)).translated(m_pos));
		// フォント
		const auto& font12 = FontAsset(FontName::Bpm);

		{
			ScopedRenderStates2D sampler(SamplerState::ClampLinear);
			TextureAsset(U"memo").drawAt({ 0, 0 });
		}

		const ScoreModel& score = notes.getScore();
		// クリアレート
		constexpr Vec2 ratePos{ -55 + 5,-63 + 4 };
		FontAsset(FontName::Level)(U"{:.2f}%"_fmt(score.clearRate)).drawAt(style, ratePos, Palette::Black);
		// 譜面製作者
		constexpr Vec2 designerPos = ratePos + Vec2{ 0, 60 };
		ContractionDrawbleString(
			font12(notes.getNotesArtistName()),
			designerPos,
			188,
			Palette::Black
		);
		// 総合ノーツ数
		constexpr Vec2 totalPos = designerPos + Vec2{ 0, 60 };
		font12(notes.getTotalNotes()).drawAt(totalPos, Palette::Black);
		// クリア情報
		constexpr Vec2 clearIconPos{ 90, -48 };
		constexpr Vec2 fcIconPos = clearIconPos + Vec2{ 0, 60 };

		if (auto gauge = notes.clearLifeGauge()) {
			constexpr Vec2 tilePos = Vec2{ 0, -102 };
			LifeGauge::GetTile(*gauge).drawAt(tilePos);
		}
		if (score.isClear) {
			TextureAsset(U"iconClear").scaled(0.5).drawAt(clearIconPos);
		}
		if (score.specialResult == SpecialResult::All_Perfect) {
			TextureAsset(U"iconAP").scaled(0.5).drawAt(fcIconPos);
		} else if (score.specialResult == SpecialResult::Full_Combo) {
			TextureAsset(U"iconFC").scaled(0.5).drawAt(fcIconPos);
		}
	}
	void MemoInfo::draw(const CourseScore& courseScore, bool drawFailure) const
	{
		Transformer2D t2d(Mat3x2::Rotate(Math::ToRadians(8)).translated(m_pos));
		{
			ScopedRenderStates2D sampler(SamplerState::ClampLinear);
			TextureAsset(U"memoCourse").drawAt({ 0, 0 });
		}
		if (courseScore.isLifeClear && courseScore.gauge > LifeGaugeKind::None) {
			constexpr Vec2 tilePos = Vec2{ 0, -102 };
			LifeGauge::GetTile(courseScore.gauge).drawAt(tilePos);
		}
		if (courseScore.isMainClear()) {
			TextureAsset(U"pass").scaled(0.3).drawAt({ 80, 10 });
		} else if (courseScore.isClear) {
			TextureAsset(U"kariPass").scaled(0.3).drawAt({ 80, 10 });
		} else if (drawFailure && courseScore.totalRate > 0 && courseScore.life == 0.0f) {
			TextureAsset(U"noPass").scaled(0.3).drawAt({ 80, 10 });
		}
		FontAsset font12os = FontAsset(FontName::Level);
		// クリアレート
		constexpr Vec2 ratePos{ -55 + 5,-63 + 4 };
		font12os(U"{:.2f}%"_fmt(courseScore.totalRate)).drawAt(style, ratePos, Palette::Black);
		// ライフ
		constexpr Vec2 lifePos = ratePos + Vec2{ 0, 60 };
		font12os(U"{:.2f}%"_fmt(courseScore.life)).drawAt(style, lifePos, Palette::Black);
	}
}
