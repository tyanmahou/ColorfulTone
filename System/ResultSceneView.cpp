#include "ResultSceneView.hpp"
#include "ResultScene.hpp"
#include "Useful.hpp"
#include "SharedDraw.hpp"
#include "FontKinetic.h"
#include "Util.h"
#include "ResultRank.h"

namespace
{
	constexpr Vec2 graphPos{ 85, 365 };
	constexpr Vec2 graphSize{ 380, 135 };
	Array<Vec2> CreateJudgeGrapghPoints(const Score& result, uint32 total)
	{
		Array<Vec2> points;
		uint32 count = 1;
		points.emplace_back(graphPos.x, graphPos.y - 7.0);
		for (auto&& judge : result.m_judgeHistory)
		{
			double y = graphPos.y - 7.0 + graphSize.y * judge / static_cast<double>(Score::Miss);

			if (total == 0)
			{
				break;
			}
			points.emplace_back(
				graphPos.x + graphSize.x* count / total,
				y
			);
			++count;
		}
		return std::move(points);
	}
	Array<Vec2> CreateLifeGraphPoints(const Score& result, uint32 total)
	{
		Array<Vec2> points;
		uint32 count = 0;
		for (auto&& life : result.m_lifeHistory)
		{
			double y = graphPos.y + graphSize.y - graphSize.y * life / 10000.0;

			if (total == 0)
			{
				points.emplace_back(graphPos.x, y);
				points.emplace_back(graphPos.x + graphSize.x, y);
				break;
			}
			points.emplace_back(
				graphPos.x + graphSize.x* count / total,
				y
			);
			++count;
		}
		return std::move(points);
	}
	void DrawGraph(const Array<Vec2>& lifes, const Array<Vec2>& judges, double t)
	{
		Graphics2D::SetSamplerState(SamplerState::WrapPoint);
		TextureAsset(L"memo2").draw(0, 350);
		constexpr Vec2 posO = graphPos + Vec2{ -5,graphSize.y + 5 };
		Line(posO, graphPos + Vec2{ -5,-5 }).drawArrow(1, Vec2{ 5.0,5.0 }, Palette::Black);
		Line(posO, graphPos + Vec2{ graphSize.x + 5,graphSize.y + 5 }).drawArrow(1, Vec2{ 5.0,5.0 }, Palette::Black);

		util::StencilMask(
			[&]() {
				RectF(graphPos + Vec2{0,-7}, { graphSize.x * t, graphSize.y }).draw();
			},
			[&]() {
				for (uint32 index = 0; index < judges.size() - 1; ++index)
				{
					Line(judges[index], judges[index + 1]).draw(0.5, ColorF(1, 0, 0, 0.5));
				}
				for (uint32 index = 0; index < lifes.size() - 1; ++index)
				{
					Line(lifes[index], lifes[index + 1]).draw(0.5, Palette::Blue);
				}
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
			.setPos({ 500,130 })
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

	void DrawScore(const ScoreModel& score, double t)
	{
		const FontAsset font20bs = FontAsset(L"20b_s");

		// レート
		font20bs(Format(L"{:.2f}%"_fmt, score.clearRate * t).padLeft(7, L' '))
			.drawKinetic(485, 215, FontKinetic::DeleteSpace, Palette::Black);


		TextureAsset(ResultRank::GetRankTextureName(score.clearRate))
			.scale(0.4)
			.drawAt(395, 465);

		constexpr Vec2 clearIconPos{ 355, 265 };
		constexpr Vec2 fcIconPos = clearIconPos + Vec2{ 65, 0 };

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
	Array<Vec2> m_graphLifePos;
	Array<Vec2> m_graphJudgePos;
	EasingController<double> m_animeTime;

public:
	Impl(const ResultScene* const scene) :
		m_pScene(scene),
		m_animeTime(0, 1.0, Easing::Quart, 1000)
	{}

	void init()
	{
		m_graphLifePos = ::CreateLifeGraphPoints(
			m_pScene->getResult(),
			m_pScene->getNotes().getTotalNotes()
		);
		m_graphJudgePos = ::CreateJudgeGrapghPoints(
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
		::DrawGraph(m_graphLifePos, m_graphJudgePos, animationTime);
		// リザルト
		::DrawResult(
			m_pScene->getResult(),
			notes.getTotalNotes(),
			animationTime
		);
		// スコア
		::DrawScore(score, animationTime);

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
