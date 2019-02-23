#include "MusicSelectView.hpp"
#include "MusicSelect.hpp"
#include "Fade.h"
#include "PlayKey.h"
#include "SharedDraw.hpp"
#include "ResultRank.h"
#include "GenreManager.h"

namespace
{
	using SortMode = MusicSelect::SortMode;
	using Action = MusicSelect::Action;
	using AllNotesInfo = MusicSelect::AllNotesInfo;

	void DrawHighSpeedDemo(const MusicSelect*const pScene)
	{
		const auto& highSpeedDemo = pScene->getHighSpeedDemo();
		float scrollRate = pScene->getScrollRate();
		auto select = MusicSelect::GetSelectInfo();

		auto& musics = pScene->getMusics();
		if (!musics.size())
		{
			return;
		}
		auto& music = musics[select.music];

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
			FontAsset(L"bpm")(tmp, L"=", result.value()).drawKinetic(10, 530, kineticFunction);
		}
		if (Input::KeyControl.pressed)
		{
			highSpeedDemo.draw(music.getMinSoundBeat(), music.getMaxSoundBeat(), scrollRate);
		}
	}

	void DrawMusicInfo(const Action action, const GenreData* pGenre, const MusicData* pMusic)
	{
		SharedDraw::JacketInfo infoView;

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
			const String& bpm = L"BPM" + Pad(pMusic->getBPM(), { 5,L' ' });
			infoView
				.drawTitle(pMusic->getMusicName())
				.drawSub(pMusic->getArtistAndAuthority())
				.drawDetail(bpm);
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
		SharedDraw::Sticky(&sortName.at(mode), &genreName);
	}
	// 譜面情報
	void DrawNotesInfo(const NotesData& notes, double offset)
	{
		util::Transformer2D t2d(Mat3x2::Rotate(Math::Radians(8), { 630, 480 }).translate({ 0, offset }));
		// フォント
		const auto& font12 = FontAsset(L"bpm");
		Graphics2D::SetSamplerState(SamplerState::ClampLinear);
		TextureAsset(L"memo").drawAt({ 630,480 });
		Graphics2D::SetSamplerState(SamplerState::Default2D);

		const ScoreModel& score = notes.getScore();
		// クリアレート
		constexpr Vec2 ratePos(575, 417);
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
		constexpr Vec2 clearIconPos(720, 432);
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
			const Rect rect(0, 500 - w* size + w * i, w, w);
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
	void DrawJacket(Action action, const GenreData* pGenre, const MusicData* pMusic, double shaderTimer = 0.0)
	{
		const auto* pTexture = ::GetJacketTexture(action, pGenre, pMusic);
		if (!pTexture)
		{
			return;
		}
		// ジャケ絵描画
		Fade::DrawCanvas(shaderTimer, [=, &pTexture]()
		{
			pTexture
				->resize(Constants::JacketWidth, Constants::JacketWidth)
				.drawAt(Constants::JacketCenter, 250);
		});
	}
}
class MusicSelectView::Impl
{
	const MusicSelect*const m_pScene;
	double m_shaderTimer = 0.0;
	EasingController<double> m_memoOffset;
	SharedDraw::DrawBGLight m_lights;

public:
	Impl(const MusicSelect*const scene) :
		m_pScene(scene),
		m_memoOffset(300, 0, Easing::Back, 1000)
	{}

	void resetShaderTimer()
	{
		m_shaderTimer = 0;
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
		m_shaderTimer += 0.025;
	}

	void draw() const
	{
		TextureAsset(L"canvasBg").draw();
		m_lights.draw();
		TextureAsset(L"label").draw(0, 500);

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
		// ジャケ絵描画
		::DrawJacket(action, pGenre, pMusic, m_shaderTimer);

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
		// 譜面情報
		if (pNotes && select.level < pNotes->size())
		{
			::DrawNotesInfo((*pNotes)[select.level], m_memoOffset.easeOut());
		}

		::DrawMusicInfo(action, pGenre, pMusic);

		// ソートとジャンル名表示
		if (pGenre)
		{
			::DrawSortAndGenre(select.sortMode, pGenre->getName());
		}
		// ハイスピ
		::DrawHighSpeedDemo(m_pScene);
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