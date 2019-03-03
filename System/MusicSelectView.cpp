#include "MusicSelectView.hpp"
#include "MusicSelect.hpp"
#include "Fade.h"
#include "PlayKey.h"
#include "SharedDraw.hpp"
#include "ResultRank.h"
#include "GenreManager.h"
#include "HighSpeedDemo.h"
namespace
{
	using SortMode = MusicSelect::SortMode;
	using Action = MusicSelect::Action;
	using AllNotesInfo = MusicSelect::AllNotesInfo;

	void DrawMusicInfo(const SharedDraw::JacketInfo& infoView, const Action action, const GenreData* pGenre, const MusicData* pMusic)
	{
		infoView.drawLine();

		if (action == MusicSelect::Action::GenreSelect)
		{
			if (!pGenre)
			{
				return;
			}
			infoView.drawTitle(pGenre->getName());
		}
		else
		{
			if (!pMusic)
			{
				return;
			}
			//BPM
			infoView
				.drawTitle(pMusic->getMusicName())
				.drawSub(pMusic->getArtistAndAuthority())
				.drawDetailRight(pMusic->getFormattedBpm());
		}
	}
	void DrawSortAndGenre(SortMode mode, const String& genreName)
	{
		static const std::unordered_map<SortMode, String> sortName{
			{SortMode::FileName, L"ファイル順"},
			{SortMode::MusicName, L"曲名順"},
			{SortMode::ArtistName, L"アーティスト名順"},
			{SortMode::LastUpdateAt, L"更新日時順"},
		};
		SharedDraw::Sticky(&genreName, &sortName.at(mode));
	}

	String NotesToLevel(const NotesData& notes)
	{
		return Format(notes.getLevel());
	}
	String NotesToRank(const NotesData& notes)
	{
		return ResultRank::GetRankTextureName(notes.getScore().clearRate);
	}
	void DrawAllNotesInfo(const Array<NotesData>* pNotes, AllNotesInfo drawMode = AllNotesInfo::Level)
	{
		if (!pNotes)
		{
			return;
		}
		const auto& notes = *pNotes;
		constexpr int w = 30;
		auto toStringMap = drawMode == AllNotesInfo::Level ?
			::NotesToLevel :
			::NotesToRank;

		for (unsigned i = 0, size = notes.size(); i < size; ++i)
		{
			const Rect rect(0, 500 - w * size + w * i, w, w);
			rect.draw(HSV(i*360.0 / size, 0.3, 1)).drawFrame(1, 0, HSV(i*360.0 / size, 1, 1));
			FontAsset(L"info")(toStringMap(notes[i])).drawCenter(rect.center, Palette::Black);
		}
	}

	const Texture* GetJacketTexture(Action action, const GenreData* pGenre, const MusicData* pMusic)
	{
		if (pGenre && action == MusicSelect::Action::GenreSelect)
		{
			return &pGenre->getTexture();
		}
		if (pMusic)
		{
			return &pMusic->getTexture();
		}
		return nullptr;
	}
	//ジャケット描画
	void DrawJacket(Action action, const GenreData* pGenre, const MusicData* pMusic, uint32 level)
	{
		const auto* pTexture = ::GetJacketTexture(action, pGenre, pMusic);
		if (!pTexture)
		{
			return;
		}
		const Color color = action == Action::LevelSelect ? (*pMusic)[level].getColor() : Palette::White;
		// ジャケ絵描画
		const Vec2 pos{ Constants::JacketCenter, 250 };
		const Vec2 size{ 310,310 };
		RectF(pos - size / 2.0, size).draw(color);
		pTexture
			->resize(size)
			.rotate(Math::Radians(-7.0))
			.drawAt(Constants::JacketCenter, 250);
	}
}
class MusicSelectView::Impl
{
	const MusicSelect*const m_pScene;

	EasingController<double> m_bgTimer;
	EasingController<double> m_memoOffset;
	SharedDraw::DrawBGLight m_lights;

public:
	Impl(const MusicSelect*const scene) :
		m_pScene(scene),
		m_bgTimer(0, 1.0, Easing::Circ, 1000),
		m_memoOffset(300, 0, Easing::Back, 1000)
	{
		m_bgTimer.start();
	}

	void resetShaderTimer()
	{
		m_bgTimer.reset();
		m_bgTimer.start();
	}

	void onChangeAction()
	{
		const auto change = m_pScene->getChangeAction();
		const auto previous = change.first;
		const auto current = change.second;
		if (previous == Action::MusicSelect && current == Action::LevelSelect)
		{
			m_memoOffset.reset();
			m_memoOffset.start();
		}
		else if (previous == Action::LevelSelect && current == Action::MusicSelect)
		{
			m_memoOffset.reset();
			m_memoOffset.restart();
		}
	}

	void update()
	{
		m_lights.update();
	}

	void draw() const
	{
		TextureAsset(L"canvasBg").draw();
		m_lights.draw();

		auto select = MusicSelect::GetSelectInfo();

		const auto& genres = GenreManager::GetGenreDates();
		const GenreData* pGenre = genres.size()
			? &genres[select.genre] : nullptr;

		const auto& musics = m_pScene->getMusics();

		const MusicData* pMusic = musics.size()
			? &musics[select.music] : nullptr;

		const Array<NotesData>* pNotes = pMusic
			? &pMusic->getNotesData() : nullptr;

		const auto action = m_pScene->getAction();

		// ラベル
		SharedDraw::JacketInfo jacketInfo;
		if (pMusic && action != Action::GenreSelect)
		{
			jacketInfo.drawLabel(pMusic->getTexture(), m_bgTimer.easeOut());
		}

		// ジャケ絵描画
		::DrawJacket(action, pGenre, pMusic, select.level);

		const int moveSelect = m_pScene->getMoveSelect();

		static EasingController<double> easingAnime(0.0, -30.0, Easing::Linear, 100);
		if (moveSelect)
		{
			easingAnime.reset();
			easingAnime.start();
		}
		const double offset = easingAnime.isActive() ?
			easingAnime.easeInOut() :
			easingAnime.getEnd();


		if (action == MusicSelect::Action::GenreSelect)
		{
			SharedDraw::Select<GenreData>()
				.setOffset(offset)
				.setDrawble([](const GenreData& g, Vec2 pos) {
				g.getTexture().resize(50, 50).drawAt(pos + Vec2{ 37,30 });
			}).draw(
				genres,
				select.genre,
				[](const GenreData& g)->decltype(auto) {return g.getName(); }
			);
		}
		else if (action == MusicSelect::Action::MusicSelect)
		{
			SharedDraw::Select<MusicData>()
				.setOffset(offset)
				.setDrawble([](const MusicData& m, Vec2 pos) {
				m.getTexture().resize(50, 50).drawAt(pos + Vec2{ 37, 30 });
			}).draw(
				musics,
				select.music,
				[](const MusicData& m)->decltype(auto) {return m.getMusicName(); }
			);
		}
		else if (action == MusicSelect::Action::LevelSelect)
		{
			SharedDraw::Select<NotesData>()
				.setOffset(offset)
				.setWidth(206)
				.setColorCallBack([](const NotesData&n) {
				return n.getColor();
			}).setDrawble([](const NotesData& n, Vec2 pos) {
				FontAsset(L"level")(n.getLevel()).drawCenter(pos + Vec2{ 40, 25 });
				TextureAsset(ResultRank::GetRankTextureName(n.getScore().clearRate)).scale(0.1).drawAt(pos + Vec2{ 320, 25 });
			}).draw(
				*pNotes,
				select.level,
				[](const NotesData& n)->decltype(auto) {return n.getLevelName(); }
			);
		}

		if (action != Action::GenreSelect)
		{
			::DrawAllNotesInfo(pNotes, select.notesInfo);
		}

		jacketInfo.drawLabel();
		// 譜面情報
		if (pNotes && select.level < pNotes->size())
		{
			SharedDraw::MemoInfo()
				.setPos(SharedDraw::MemoInfo::DefaultPos + Vec2{ 0, m_memoOffset.easeOut() })
				.draw((*pNotes)[select.level]);
		}

		::DrawMusicInfo(jacketInfo, action, pGenre, pMusic);

		// ソートとジャンル名表示
		if (pGenre)
		{
			::DrawSortAndGenre(select.sortMode, pGenre->getName());
		}
		// ハイスピ
		if (action != Action::GenreSelect && pMusic)
		{
			SharedDraw::HighSpeed(
				m_pScene->getHighSpeedDemo(),
				*pMusic,
				m_pScene->getScrollRate()
			);
		}
	}
};
MusicSelectView::MusicSelectView(const MusicSelect*const scene) :
	m_pImpl(std::make_shared<Impl>(scene))
{}

MusicSelectView::~MusicSelectView()
{}

void MusicSelectView::resetShaderTimer()
{
	m_pImpl->resetShaderTimer();
}

void MusicSelectView::onChangeAction()
{
	m_pImpl->onChangeAction();
}

void MusicSelectView::update()
{
	m_pImpl->update();
}

void MusicSelectView::draw() const
{
	m_pImpl->draw();
}