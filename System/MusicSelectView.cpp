#include "MusicSelectView.hpp"
#include "MusicSelect.hpp"
#include "Fade.h"
#include "PlayKey.h"
#include "SharedDraw.hpp"
#include"ResultRank.h"

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
MusicSelectView::MusicSelectView(const MusicSelect*const scene):
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
		music.getTexture().resize(400, 400).draw(50, 50);
	});

	const int moveSelect = m_pScene->getMoveSelect();
	const auto action = m_pScene->getAction();
	if (action == MusicSelect::Action::MusicSelect)
	{
		SharedDraw::Select<MusicData>(
			musics,
			select.music,
			moveSelect,
			[](const MusicData& m)->decltype(auto) {return m.getMusicName(); }
		);
	}
	else if(action == MusicSelect::Action::LevelSelect)
	{
		SharedDraw::Select<NotesData>(
			music.getNotesData(),
			select.level,
			moveSelect,
			[](const NotesData& n)->decltype(auto) {
				return n.getLevelName();
			},
			206,
			[](const NotesData&n){
				return n.getColor();
			},
			[](const NotesData& n, Vec2 pos) {
				FontAsset(L"level")(n.getLevel()).drawCenter(pos + Vec2{ 40, 25 });
				TextureAsset(ResultRank::getRankTextureName(n.clearRate)).scale(0.1).drawAt(pos + Vec2{ 320, 25 });
			}
		);
	}
	::DrawHighSpeedDemo(m_pScene);
}