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
	constexpr double jacketWidth = 350;  // 曲情報の幅
	constexpr double jacketCenter = 220; // 曲情報の中心

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

	void DrawStringOnMusicInfo(
		const String* pTitle = nullptr,
		const String* pSub = nullptr,
		const String* pDetail = nullptr
	) {
		// フォント
		const auto& font12 = FontAsset(L"bpm");
		const auto& font16b = FontAsset(L"selectMusics");

		if (pTitle)
		{
			//タイトル
			util::ContractionDrawbleString(
				font16b(*pTitle),
				{ jacketCenter,440 },
				jacketWidth,
				Palette::Black
			);
		}
		if (pSub)
		{
			// サブタイトル
			util::ContractionDrawbleString(
				font12(*pSub),
				{ jacketCenter,475 },
				jacketWidth,
				Palette::Black
			);
		}
		if (pDetail)
		{
			// 詳細
			const int wSize = 12;
			const int width = wSize * (*pDetail).length;

			const auto kinetic = [wSize](KineticTypography& k)
			{
				k.pos.x = k.origin.x + wSize * k.index;
			};
			font12(*pDetail).drawKinetic(
				{ jacketCenter + jacketWidth / 2.0 - width, 495 },
				kinetic,
				Palette::Black
			);
		}
	}
	void DrawMusicInfo(const Action action, const MusicData& music, const GenreData& genre)
	{
		TextureAsset(L"line").drawAt({ jacketCenter,475 });

		if (action == MusicSelect::Action::GenreSelect)
		{
			::DrawStringOnMusicInfo(&genre.getName());
		}
		else
		{
			//作曲家 + 出典
			String artistName = music.getArtistName();
			const auto& authority = music.getAuthority();
			if (authority.has_value())
			{
				artistName += L" / " + authority.value();
			}
			//BPM
			const String& bpm = L"BPM" + Pad(music.getBPM(), { 5,L' ' });
			::DrawStringOnMusicInfo(
				&music.getMusicName(),
				&artistName,
				&bpm
			);
		}
	}
	void DrawSortAndGenre(SortMode mode, const String& genreName)
	{
		// フォント
		const auto& font = FontAsset(L"bpm");

		static const std::unordered_map<SortMode, String> sortName{
			{SortMode::FileName, L"ファイル順"},
			{SortMode::MusicName, L"曲名順"},
			{SortMode::ArtistName, L"アーティスト名順"},
			{SortMode::LastUpdateAt, L"更新日時順"},
		};
		// ソート
		{
			util::Transformer2D t2d(Mat3x2::Rotate(Math::Radians(-20)).translate({ 25, 95 }));
			TextureAsset(L"sticky_red").draw();
			util::ContractionDrawbleString(
				font(sortName.at(mode)),
				{ 125,25 },
				175,
				Palette::Black
			);
		}
		// 選択中ジャンル
		{
			util::Transformer2D t2d(Mat3x2::Rotate(Math::Radians(-30)).translate({ -25, 95 }));
			TextureAsset(L"sticky").draw();
			util::ContractionDrawbleString(
				font(genreName),
				{ 125,25 },
				175,
				Palette::Black
			);
		}
	}
	// 譜面情報
	void DrawNotesInfo(const NotesData& notes, double offset)
	{
		util::Transformer2D t2d(Mat3x2::Rotate(Math::Radians(8), { 630, 480 }).translate({0, offset}));
		// フォント
		const auto& font12 = FontAsset(L"bpm");
		Graphics2D::SetSamplerState(SamplerState::ClampLinear);
		TextureAsset(L"memo").drawAt({ 630,480 });
		Graphics2D::SetSamplerState(SamplerState::Default2D);

		// クリアレート
		constexpr Vec2 ratePos(575, 417);
		FontAsset(L"level")(L"{:.2f}%"_fmt, notes.clearRate).drawCenter(ratePos, Palette::Black);
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
		if (notes.isClear)
		{
			TextureAsset(L"iconClear").scale(0.5).drawAt(clearIconPos);
		}
		if (notes.specialResult == SpecialResult::All_Perfect)
		{
			TextureAsset(L"iconAP").scale(0.5).drawAt(fcIconPos);
		}
		else if (notes.specialResult == SpecialResult::Full_Combo)
		{
			TextureAsset(L"iconFC").scale(0.5).drawAt(fcIconPos);
		}
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
		const GenreData& genre = genres[select.genre];

		const auto& musics = m_pScene->getMusics();

		if (musics.size())
		{
			const MusicData& music = musics[select.music];
			const auto& notes = music.getNotesData();

			const auto action = m_pScene->getAction();
			const auto& jacketTexture = action == MusicSelect::Action::GenreSelect
				? genre.getTexture() : music.getTexture();

			// ジャケ絵描画
			Fade::DrawCanvas(m_shaderTimer, [=, &jacketTexture]()
			{
				jacketTexture.resize(jacketWidth, jacketWidth).drawAt(jacketCenter, 250);
			});

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
					TextureAsset(ResultRank::getRankTextureName(n.clearRate)).scale(0.1).drawAt(pos + Vec2{ 320, 25 });
				}).draw(
					notes,
					select.level,
					[](const NotesData& n)->decltype(auto) {return n.getLevelName(); }
				);
			}
			// 譜面情報
			if (select.level < notes.size())
			{
				::DrawNotesInfo(notes[select.level], m_memoOffset.easeOut());
			}

			::DrawMusicInfo(action, music, genre);
		}
		// ソートとジャンル名表示
		::DrawSortAndGenre(select.sortMode, genre.getName());

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