#include "ResultSceneView.hpp"
#include "ResultScene.hpp"
#include "Useful.hpp"
#include "SharedDraw.hpp"
#include "FontKinetic.h"
#include "Util.h"
#include "ResultRank.h"
#include "EasingSequence.hpp"

namespace
{
	namespace ResultMarker {
		constexpr Color Perfect{ 70, 255, 200 , 128 };
		constexpr Color Great{ 140, 255, 140 , 128 };
		constexpr Color Good{ 255, 255, 125 , 128 };
		constexpr Color Miss{ 255, 140, 140 , 128 };
	}
	constexpr RectF GraphRect()
	{
		return { 90, 370, 370, 120 };
	}
	using Judges = Array<std::pair<RectF, Color>>;
	void CreateJudgeGrapgh(Judges& judges, const Score& result, uint32 total)
	{
		constexpr auto graphRect = ::GraphRect();

		if (total == 0)
		{
			return;
		}
		const auto& history = result.m_judgeHistory;
		//　分割数計算
		constexpr uint8 divide = 20;

		for (uint8 row = 0; row < divide; ++row)
		{
			std::array<uint32, Score::TERM> rowJudges{ 0 };
			uint32 start = total * row / divide;
			uint32 end = total * (row + 1) / divide;

			for (uint32 index = start; index < end || index == start; ++index)
			{
				if (index >= history.size()) {
					break;
				}
				++rowJudges[history[index]];
			}
			const uint32 rowTotal = end > start ? end - start : 1;
			const auto rate = ResultRank::CalcBaseRate(rowJudges, rowTotal);
			double height = graphRect.h * static_cast<double>(rate);
			const Vec2 pos{ graphRect.x + graphRect.w*row / divide,graphRect.y + graphRect.h - height };
			const Vec2 size{ graphRect.w / divide - 1.0, height };
			Color color;
			if (rowJudges[Score::Miss])
			{
				color = ResultMarker::Miss;
			}
			else if (rowJudges[Score::Good])
			{
				color = ResultMarker::Good;
			}
			else if (rowJudges[Score::Great])
			{
				color = ResultMarker::Great;
			}
			else {
				color = ResultMarker::Perfect;
			}
			color.setAlpha(128);

			judges.push_back(std::make_pair(RectF{ pos, size }, color));
		}
	}
	void CreateLifeGraph(LineString& lifes, const Score& result, uint32 total)
	{
		constexpr auto graphRect = ::GraphRect();

		const RectF lifeRect{
			graphRect.x + 5,
			graphRect.y + 5,
			graphRect.w - 10,
			graphRect.h - 10
		};
		Array<Vec2> points;
		uint32 count = 0;
		for (auto&& life : result.m_lifeHistory)
		{
			double y = lifeRect.y + lifeRect.h - lifeRect.h * life / 10000.0;

			if (total == 0)
			{
				lifes.push_back({ lifeRect.x, y });
				lifes.push_back({ lifeRect.x + lifeRect.w, y });
				break;
			}
			lifes.push_back({
				lifeRect.x + lifeRect.w * count / total,
				y
				});
			++count;
		}
	}
	void DrawGraph(const LineString& graphLifes, const Judges& judges, double t)
	{
		constexpr auto graphRect = ::GraphRect();

		Graphics2D::SetSamplerState(SamplerState::WrapPoint);
		TextureAsset(L"memo2").draw(0, 350);

		graphRect.drawFrame(1, 0, Palette::Black);
		util::StencilMask(
			[&]() {
			RectF(graphRect.pos, { graphRect.w * t, graphRect.h }).draw();
		},
			[&]() {
			for (auto && rectColor : judges)
			{
				rectColor.first.draw(rectColor.second);
			}
			graphLifes.draw(1, Palette::Blue);
		},
			StencilFunc::Equal
			);
		Graphics2D::SetSamplerState(SamplerState::Default2D);
	}
	void  DrawMusicInfo(const NotesData& notes, double t)
	{
		constexpr Vec2 jacketSize{ 240 ,240 };
		constexpr Vec2 jacketPos{ 170, 200 };
		RectF(jacketPos - jacketSize / 2.0, jacketSize).draw(notes.getColor());

		const MusicData& music = *notes.getMusic();
		music
			.getTexture()
			.resize(jacketSize)
			.rotate(Math::Radians(-7.0))
			.drawAt(jacketPos);

		SharedDraw::JacketInfo infoView;
		infoView
			.setPos({ 500,140 })
			.drawLabel(music.getTexture(), t)
			.drawLabel()
			.drawLine()
			.drawTitle(music.getMusicName())
			.drawSub(music.getArtistAndAuthority())
			.drawDetail(notes.getLevelNameAndLevel());
	}

	void DrawCount(const Vec2& pos, const String& name, uint32 count)
	{
		constexpr Vec2 basePos{ 530, 275 };
		const FontAsset font8os = FontAsset(L"8o_s");
		font8os(name).drawKinetic(pos.x, pos.y, FontKinetic::DeleteSpace);
		font8os(Pad(count, { 6,L' ' })).drawKinetic(pos.x + 120, pos.y, FontKinetic::DeleteSpace);
	}
	void DrawResult(const Score& result, const uint32 total, double t)
	{
		// コンボと判定
		constexpr Vec2 basePos{ 530, 285 };
		::DrawCount(basePos, L"MAX COMBO", result.m_maxCombo* t);

		Line({ 45,335 }, { 755,335 }).draw(Palette::Black);

		const auto& judgeCount = result.m_judgeCount;
		constexpr double offset = 75;
		RectF(basePos + Vec2{ -10,offset + 10 }, { 120,10 }).draw(ResultMarker::Perfect);
		RectF(basePos + Vec2{ -10,offset + 30 + 10 }, { 120,10 }).draw(ResultMarker::Great);
		RectF(basePos + Vec2{ -10,offset + 60 + 10 }, { 120,10 }).draw(ResultMarker::Good);
		RectF(basePos + Vec2{ -10,offset + 90 + 10 }, { 120,10 }).draw(ResultMarker::Miss);
		::DrawCount(basePos + Vec2{ 0, offset }, L"PERFECT", judgeCount[Score::Perfect] * t);
		::DrawCount(basePos + Vec2{ 0, offset + 30 }, L"GREAT", judgeCount[Score::Great] * t);
		::DrawCount(basePos + Vec2{ 0, offset + 60 }, L"GOOD", judgeCount[Score::Good] * t);
		::DrawCount(basePos + Vec2{ 0, offset + 90 }, L"MISS", judgeCount[Score::Miss] * t);
		::DrawCount(basePos + Vec2{ 0, offset + 120 }, L"TOTAL", total * t);
	}

	void DrawScore(const ScoreModel& score, bool isNewRecord, double t, double t2)
	{
		const FontAsset font20bs = FontAsset(L"20b_s");

		// レート
		font20bs(Format(L"{:.2f}%"_fmt, score.clearRate * t).padLeft(7, L' '))
			.drawKinetic(485, 215, FontKinetic::DeleteSpace, Palette::Black);

		const Color alpha = ColorF(1.0, t2);
		if (isNewRecord)
		{
			TextureAsset(L"newRecord").drawAt(680 - (1.0 - t2) * 10, 235, alpha);
		}
		TextureAsset(ResultRank::GetRankTextureName(score.clearRate))
			.scale(0.4)
			.drawAt(400, 250 -(1.0 - t2) * 10,alpha);

		constexpr Vec2 clearIconPos{ 715 ,100 };
		constexpr Vec2 fcIconPos = clearIconPos + Vec2{ 0, 60 };
		if (score.isClear)
		{
			TextureAsset(L"iconClear").scale(0.5).drawAt(clearIconPos, alpha);
		}
		if (score.specialResult == SpecialResult::All_Perfect)
		{
			TextureAsset(L"iconAP").scale(0.5).drawAt(fcIconPos, alpha);
		}
		else if (score.specialResult == SpecialResult::Full_Combo)
		{
			TextureAsset(L"iconFC").scale(0.5).drawAt(fcIconPos, alpha);
		}
	}
}

class ResultSceneView::Impl
{
private:
	const ResultScene* const m_pScene;
	SharedDraw::DrawBGLight m_lights;
	LineString m_graphLife;
	Judges m_graphJudge;
	EasingSequence m_timers;
public:
	Impl(const ResultScene* const scene) :
		m_pScene(scene),
		m_timers({
			{0, 1.0, Easing::Circ, 1000},
			{0, 1.0, Easing::Linear, 500}
		})
	{}

	void init()
	{
		::CreateLifeGraph(
			m_graphLife,
			m_pScene->getResult(),
			m_pScene->getNotes().getTotalNotes()
		);
		::CreateJudgeGrapgh(
			m_graphJudge,
			m_pScene->getResult(),
			m_pScene->getNotes().getTotalNotes()
		);
	}
	void update()
	{
		m_timers.update();
		m_lights.update();
	}

	void draw()const
	{
		// BG
		TextureAsset(L"canvasBg").draw();
		m_lights.draw();

		const ScoreModel& score = m_pScene->getScore();
		const NotesData& notes = m_pScene->getNotes();
		const double animationTime = m_timers[0].easeOut();
		const double scoreAnimeTime = m_timers[1].easeOut();
		::DrawMusicInfo(notes, animationTime);

		// グラフ
		::DrawGraph(m_graphLife, m_graphJudge, animationTime);
		// リザルト
		::DrawResult(
			m_pScene->getResult(),
			notes.getTotalNotes(),
			animationTime
		);
		// スコア
		::DrawScore(score, m_pScene->isNewRecord(), animationTime, scoreAnimeTime);

		static const String sceneName = L"RESULT";
		SharedDraw::Sticky(
			&sceneName,
			nullptr // TODO course
		);
	}
};

ResultSceneView::ResultSceneView(const ResultScene* const scene) :
	m_impl(std::make_shared<Impl>(scene))
{}

void ResultSceneView::init()
{
	m_impl->init();
}

void ResultSceneView::update() const
{
	m_impl->update();
}

void ResultSceneView::draw() const
{
	m_impl->draw();
}
