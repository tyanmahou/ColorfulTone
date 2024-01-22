#include <scenes/Scene/MusicSelect/MusicSelectSceneView.hpp>
#include <scenes/Scene/MusicSelect/MusicSelectScene.hpp>
#include <commons/Constants.hpp>
#include <commons/FontName.hpp>
//#include "Fade.h"
//#include "PlayKey.h"
#include <scenes/utils/SharedDraw.hpp>
#include <core/Data/Score/ResultRank.hpp>
#include <core/Data/Genre/GenreManager.hpp>
#include <scenes/Scene/KeyConfig/ConfigMain.hpp>
#include <Siv3D.hpp>

namespace
{
	using namespace ct;

	using SortMode = MusicSelectScene::SortMode;
	using Action = MusicSelectScene::Action;
	using AllNotesInfo = MusicSelectScene::AllNotesInfo;

	void DrawMusicInfo(const SharedDraw::JacketInfo& infoView, const Action action, const GenreData* pGenre, const MusicData* pMusic)
	{
		infoView.drawLine();

		if (action == Action::GenreSelect)
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
			{SortMode::FileName, U"ファイル順"},
			{SortMode::MusicName, U"曲名順"},
			{SortMode::ArtistName, U"アーティスト名順"},
			{SortMode::LastUpdateAt, U"更新日時順"},
		};
		SharedDraw::Sticky(&genreName, &sortName.at(mode));
	}

	String NotesToLevel(const NotesData& notes)
	{
		if (notes.getStarLv() != StarLv::None) {
			if (Time::GetMillisec() % 4000 <= 2000) {
				return Format(ToStr(notes.getStarLv()));
			} else {
				return Format(notes.getLevel());
			}
		}
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

		for (size_t i = 0, size = notes.size(); i < size; ++i)
		{
			const Rect rect(0, 500 - w * size + w * i, w, w);
			rect.draw(HSV(i*360.0 / size, 0.3, 1)).drawFrame(1, 0, HSV(i*360.0 / size, 1, 1));

			ContractionDrawbleString(
				FontAsset(FontName::Info)(toStringMap(notes[i])),
				rect.center(),
				rect.w,
				Palette::Black
			);
		}
	}

	Optional<Texture> GetJacketTexture(Action action, const GenreData* pGenre, const MusicData* pMusic)
	{
		if (pGenre && action == Action::GenreSelect)
		{
			return pGenre->getTexture();
		}
		if (pMusic)
		{
			return pMusic->getTexture();
		}
		return s3d::none;
	}
	//ジャケット描画
	void DrawJacket(Action action, const GenreData* pGenre, const MusicData* pMusic, uint32 level)
	{
		const Optional<Texture> pTexture = ::GetJacketTexture(action, pGenre, pMusic);
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
			->resized(size)
			.rotated(Math::ToRadians(-7.0))
			.drawAt(pos);
		if (action != Action::GenreSelect && pMusic && pMusic->isFavorite()) {
			TextureAsset(U"favorite").drawAt(pos + Vec2{ 155, -155 });
		}
	}
}

namespace ct
{
	class MusicSelectSceneView::Impl
	{
		const MusicSelectScene* const m_pScene;

		EasingAB<double> m_bgTimer;
		EasingAB<double> m_memoOffset;
		SharedDraw::DrawBGLight m_lights;

	public:
		Impl(const MusicSelectScene* const scene) :
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
			if (previous == Action::MusicSelect && current == Action::LevelSelect) {
				m_memoOffset.jumpToA();
				m_memoOffset.start();
			} else if (previous == Action::LevelSelect && current == Action::MusicSelect) {
				m_memoOffset.jumpToB();
				m_memoOffset.start();
			}
		}

		void update()
		{
			m_lights.update();
		}

		void draw() const
		{
			TextureAsset(U"canvasBg").draw();
			m_lights.draw();

			auto select = MusicSelectScene::GetSelectInfo();

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
			if (pMusic && action != Action::GenreSelect) {
				jacketInfo.drawLabel(pMusic->getTexture(), m_bgTimer.easeOut());
			}

			// ジャケ絵描画
			::DrawJacket(action, pGenre, pMusic, select.level);

			const int moveSelect = m_pScene->getMoveSelect();

			static EasingAB<double> easingAnime(0.0, -30.0, Easing::Linear, 100);
			if (moveSelect) {
				easingAnime.reset();
				easingAnime.start();
			}
			const double offset = easingAnime.isMoving() ?
				easingAnime.easeInOut() :
				easingAnime.getB();


			if (action == Action::GenreSelect) {
				SharedDraw::Select<GenreData>()
					.setOffset(offset)
					.setDrawble([](const GenreData& g, Vec2 pos) {
					g.getTexture().resized(50, 50).drawAt(pos + Vec2{ 37,30 });
						}).draw(
							genres,
							select.genre,
							[](const GenreData& g)->decltype(auto) {return g.getName(); }
						);
			} else if (action == Action::MusicSelect) {
				SharedDraw::Select<MusicData>()
					.setOffset(offset)
					.setDrawble([](const MusicData& m, Vec2 pos) {
					m.getTexture().resized(50, 50).drawAt(pos + Vec2{ 37, 30 });
						}).draw(
							musics,
							select.music,
							[](const MusicData& m)->decltype(auto) {return m.getMusicName(); }
						);
			} else if (action == Action::LevelSelect) {
				SharedDraw::Select<NotesData>()
					.setOffset(offset)
					.setWidth(206)
					.setColorCallBack([](const NotesData& n) {
					    return n.getColor();
					})
					.setDrawble([](const NotesData& n, Vec2 pos) {
					    if (n.getStarLv() != StarLv::None) {
					    	// ★レベル
					    	if (n.getStarLv() == StarLv::One) {
								FontAsset(AssetNameView(FontName::StarLv))(U"★").drawAt(pos + Vec2{ 37, 30 });
					    	} else if (n.getStarLv() == StarLv::Two) {
								FontAsset(AssetNameView(FontName::StarLv))(U"★").drawAt(pos + Vec2{ 28, 28 });
								FontAsset(AssetNameView(FontName::StarLv2))(U"★").drawAt(pos + Vec2{52, 38});
					    	}
					    
					    	constexpr Vec2 size(65, 20); // 65, 50
					    	const RectF bar(pos + Vec2{ 37, 45 } - size / 2, size);
					    	bar.draw(ColorF(0, 0.5));
					    
					    	auto&& lv = FontAsset(FontName::Info)(n.getLevel());
					    	lv.draw(bar.br() - lv.region().size - Vec2{1, 0});
					    } else {
					    	// 通常レベル
					    	ContractionDrawbleStringOutline(
					    		FontAsset(FontName::Level)(n.getLevel()),
					    		pos + Vec2{ 42, 30 },
					    		50
					    	);
					    }
					    TextureAsset(ResultRank::GetRankTextureName(n.getScore().clearRate)).scaled(0.1).drawAt(pos + Vec2{ 320, 25 });
					})
					.draw(
						*pNotes,
						select.level,
						[](const NotesData& n)->decltype(auto) {return n.getLevelName(); }
					);
			}

			if (action != Action::GenreSelect) {
				::DrawAllNotesInfo(pNotes, select.notesInfo);
			}

			jacketInfo.drawLabel();
			// 譜面情報
			if (pNotes && select.level < pNotes->size()) {
				SharedDraw::MemoInfo()
					.setPos(SharedDraw::MemoInfo::DefaultPos + Vec2{ 0, m_memoOffset.easeOut() })
					.draw((*pNotes)[select.level]);
			}

			::DrawMusicInfo(jacketInfo, action, pGenre, pMusic);

			// ソートとジャンル名表示
			if (pGenre) {
				::DrawSortAndGenre(select.sortMode, pGenre->getName());
			}
			// ハイスピ
			if (action != Action::GenreSelect && pMusic) {
				SharedDraw::HighSpeed(
					m_pScene->getHighSpeedDemo(),
					*pMusic,
					m_pScene->getScrollRate(),
					!m_pScene->getConfig().isActive()
				);
			}
			// コンフィグ
			if (m_pScene->getConfig().isActive()) {
				 Scene::Rect().draw(ColorF(0, 0.8));
				m_pScene->getConfig().draw();
			}
		}
	};
	MusicSelectSceneView::MusicSelectSceneView(const MusicSelectScene* const scene) :
		m_pImpl(std::make_shared<Impl>(scene))
	{}

	MusicSelectSceneView::~MusicSelectSceneView()
	{}

	void MusicSelectSceneView::resetShaderTimer()
	{
		m_pImpl->resetShaderTimer();
	}

	void MusicSelectSceneView::onChangeAction()
	{
		m_pImpl->onChangeAction();
	}

	void MusicSelectSceneView::update()
	{
		m_pImpl->update();
	}

	void MusicSelectSceneView::draw() const
	{
		m_pImpl->draw();
	}
}
