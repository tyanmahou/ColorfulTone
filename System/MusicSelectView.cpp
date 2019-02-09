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
	){
		// フォント
		auto& font12 = FontAsset(L"bpm");
		auto& font16b = FontAsset(L"selectMusics");

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
			const int width = wSize*(*pDetail).length;

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
	void DrawMusicInfo(const Action action,const MusicData& music, const GenreData& genre)
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
			const String& bpm = L"BPM" + Pad(music.getBPM(), {5,L' '});
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
		auto& font = FontAsset(L"bpm");

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
}
MusicSelectView::MusicSelectView(const MusicSelect*const scene) :
	m_pScene(scene)
{}


MusicSelectView::~MusicSelectView()
{
}

void MusicSelectView::draw() const
{
	TextureAsset(L"canvasBg").draw();

	TextureAsset(L"label").draw(0, 500);

	auto select = MusicSelect::GetSelectInfo();

	const auto& musics = m_pScene->getMusics();
	const MusicData& music = musics[select.music];

	const auto& genres = GenreManager::GetGenreDates();
	const GenreData& genre = genres[select.genre];


	const auto action = m_pScene->getAction();
	const auto& jacketTexture = action == MusicSelect::Action::GenreSelect
		? genre.getTexture() : music.getTexture();

	// ジャケ絵描画
	Fade::DrawCanvas(m_pScene->getShaderTimer(), [=, &jacketTexture]()
	{
		jacketTexture.resize(jacketWidth, jacketWidth).drawAt(jacketCenter, 250);
	});

	const int moveSelect = m_pScene->getMoveSelect();

	if (action == MusicSelect::Action::GenreSelect)
	{
		SharedDraw::Select<GenreData>()
			.setOnMoveSelect(moveSelect)
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
			.setOnMoveSelect(moveSelect)
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
			.setOnMoveSelect(moveSelect)
			.setWidth(206)
			.setColorCallBack([](const NotesData&n) {
			return n.getColor();
		}).setDrawble([](const NotesData& n, Vec2 pos) {
			FontAsset(L"level")(n.getLevel()).drawCenter(pos + Vec2{ 40, 25 });
			TextureAsset(ResultRank::getRankTextureName(n.clearRate)).scale(0.1).drawAt(pos + Vec2{ 320, 25 });
		}).draw(
			music.getNotesData(),
			select.level,
			[](const NotesData& n)->decltype(auto) {return n.getLevelName(); }
		);
	}

	::DrawMusicInfo(action, music, genre);
	// ソートとジャンル名表示
	::DrawSortAndGenre(select.sortMode, genre.getName());

	// ハイスピ
	::DrawHighSpeedDemo(m_pScene);
}