#include "MusicSelectView.hpp"
#include "MusicSelect.hpp"
#include "Fade.h"
#include "PlayKey.h"
#include "SharedDraw.hpp"
#include "ResultRank.h"
#include "GenreManager.h"

namespace
{
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

	auto& musics = m_pScene->getMusics();
	const uint32 musicSize = musics.size();

	auto& music = musics[select.music];
	// ジャケ絵描画
	Fade::DrawCanvas(m_pScene->getShaderTimer(), [&music]()
	{
		music.getTexture().resize(350, 350).draw(65, 40);
	});

	const int moveSelect = m_pScene->getMoveSelect();
	const auto action = m_pScene->getAction();

	if (action == MusicSelect::Action::GenreSelect)
	{
		SharedDraw::Select<GenreData>()
			.setOnMoveSelect(moveSelect)
			.setDrawble([](const GenreData& g, Vec2 pos) {
			g.getTexture().resize(50, 50).drawAt(pos + Vec2{ 37,30 });
		}).draw(
			GenreManager::GetGenreDates(),
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
	::DrawHighSpeedDemo(m_pScene);
}