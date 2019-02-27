#include "ResultSceneView.hpp"
#include "ResultScene.hpp"
#include "Useful.hpp"
#include "SharedDraw.hpp"
#include "FontKinetic.h"
#include "Util.h"
#include "ResultRank.h"

namespace
{

	constexpr RectF GraphRect()
	{
		return { 90, 370, 370, 120 };
	}
	using Judges = Array<RectF>;
	Judges CreateJudgeGrapgh(const Score& result, uint32 total)
	{
		constexpr auto graphRect = ::GraphRect();

		Judges rects;
		if (total == 0)
		{
			return rects;
		}
		const auto& history = result.m_judgeHistory;
		//　分割数計算
		constexpr uint8 divide = 20;

		for (uint8 row = 0; row < divide; ++row)
		{
			std::array<uint32, Score::TERM> rowJudges{ 0 };
			uint32 start = total * row / divide;
			uint32 end = total * (row + 1) / divide;
			for (uint32 index = start; index < end; ++index)
			{
				if (index >= history.size()) {
					break;
				}
				++rowJudges[history[index]];
			}
			double h = graphRect.h * ResultRank::CalcBaseRate(rowJudges, end - start);
			const Vec2 pos{ graphRect.x + graphRect.w*row / divide,graphRect.y + graphRect.h - h };
			const Vec2 size{ graphRect.w / divide,h };
			rects.emplace_back(pos, size);
		}
		return std::move(rects);
	}
	Array<Vec2> CreateLifeGraphPoints(const Score& result, uint32 total)
	{
		constexpr auto graphRect = ::GraphRect();

		const RectF lifeRect{
			graphRect.x + 5,
			graphRect.y + 5,
			graphRect.w - 5,
			graphRect.h - 5
		};
		Array<Vec2> points;
		uint32 count = 0;
		for (auto&& life : result.m_lifeHistory)
		{
			double y = lifeRect.y + lifeRect.h - lifeRect.h * life / 10000.0;

			if (total == 0)
			{
				points.emplace_back(lifeRect.x, y);
				points.emplace_back(lifeRect.x + lifeRect.w, y);
				break;
			}
			points.emplace_back(
				lifeRect.x + lifeRect.w * count / total,
				y
			);
			++count;
		}
		return std::move(points);
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
			for (auto && rect : judges)
			{
				Color color;
				const double h = rect.h / graphRect.h;
				if (h >= 1.0)
				{
					color = Color(70, 255, 200);
				}
				else if (h >= 0.7)
				{
					color = Color(140, 255, 140);
				}
				else if (h >= 0.5)
				{
					color = Color(255, 255, 125);
				}
				else {
					color = Color(255, 140, 140);
				}
				rect.draw(color.setAlpha(128));
			}
			graphLifes.draw(1, Palette::Blue);
		},
			StencilFunc::Equal
			);
		Graphics2D::SetSamplerState(SamplerState::Default2D);
	}
	void DrawBGJacket(const NotesData& notes, double t)
	{
		const MusicData& music = *notes.getMusic();

		util::StencilMask(
			[&]() {
			Rect(0, 0, 800 * t, 30).draw();
			Rect(800 - 800 * t, 530, 800 * t, 30).draw();
		},
			[&]() {
			music.getTexture().resize(800, 800).drawAt(Window::BaseCenter());
		},
			StencilFunc::Equal
			);
		Rect(0, 0, 800, 30).draw(Color(0, 0, 0, 100));
		Rect(0, 530, 800, 30).draw(Color(0, 0, 0, 100));
	}
	void  DrawMusicInfo(const NotesData& notes)
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
			.drawLine()
			.drawTitle(music.getMusicName())
			.drawSub(music.getArtistAndAuthority())
			.drawDetail(notes.getLevelName() + L" Lv" + Format(notes.getLevel()));
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
		::DrawCount(basePos + Vec2{ 0, offset }, L"PERFECT", judgeCount[Score::Perfect] * t);
		::DrawCount(basePos + Vec2{ 0, offset + 30 }, L"GREAT", judgeCount[Score::Great] * t);
		::DrawCount(basePos + Vec2{ 0, offset + 60 }, L"GOOD", judgeCount[Score::Good] * t);
		::DrawCount(basePos + Vec2{ 0, offset + 90 }, L"MISS", judgeCount[Score::Miss] * t);
		::DrawCount(basePos + Vec2{ 0, offset + 120 }, L"TOTAL", total * t);
	}

	void DrawScore(const ScoreModel& score, bool isNewRecord, double t)
	{
		const FontAsset font20bs = FontAsset(L"20b_s");

		// レート
		font20bs(Format(L"{:.2f}%"_fmt, score.clearRate * t).padLeft(7, L' '))
			.drawKinetic(485, 215, FontKinetic::DeleteSpace, Palette::Black);

		if (isNewRecord)
		{
			TextureAsset(L"newRecord").drawAt(680, 235);
		}
		TextureAsset(ResultRank::GetRankTextureName(score.clearRate))
			.scale(0.4)
			.drawAt(360, 250);

		constexpr Vec2 clearIconPos{ 715 ,100 };
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
}

class ResultSceneView::Impl
{
private:
	const ResultScene* const m_pScene;
	SharedDraw::DrawBGLight m_lights;
	LineString m_graphLife;
	Judges m_graphJudge;
	EasingController<double> m_animeTime;

public:
	Impl(const ResultScene* const scene) :
		m_pScene(scene),
		m_animeTime(0, 1.0, Easing::Quart, 1000)
	{}

	void init()
	{
		m_graphLife = LineString(::CreateLifeGraphPoints(
			m_pScene->getResult(),
			m_pScene->getNotes().getTotalNotes()
		));
		m_graphJudge = ::CreateJudgeGrapgh(
			m_pScene->getResult(),
			m_pScene->getNotes().getTotalNotes()
		);
	}
	void update()
	{
		if (!m_animeTime.isActive() && !m_animeTime.isEnd())
		{
			m_animeTime.start();
		}
		m_lights.update();
	}

	void draw()const
	{
		// BG
		TextureAsset(L"canvasBg").draw();
		m_lights.draw();

		const ScoreModel& score = m_pScene->getScore();
		const NotesData& notes = m_pScene->getNotes();
		const double animationTime = m_animeTime.easeOut();
		::DrawBGJacket(notes, animationTime);
		::DrawMusicInfo(notes);

		// グラフ
		::DrawGraph(m_graphLife, m_graphJudge, animationTime);
		// リザルト
		::DrawResult(
			m_pScene->getResult(),
			notes.getTotalNotes(),
			animationTime
		);
		// スコア
		::DrawScore(score, m_pScene->isNewRecord(), animationTime);

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
