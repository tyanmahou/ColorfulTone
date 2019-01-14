#include "MusicSelectView.hpp"
#include "MusicSelect.hpp"
#include "Fade.h"
#include "PlayKey.h"
#include "SharedDraw.hpp"
#include"ResultRank.h"

MusicSelectView::MusicSelectView(const MusicSelect*const scene):
	m_pScene(scene)
{}


MusicSelectView::~MusicSelectView()
{
}

void MusicSelectView::draw() const
{
	TextureAsset(L"canvasBg").draw();

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
				//m_rateFont(notesData[index].getLevel()).drawCenter(430 + 40 + offset, 10 + 15 + 60 * i);
				TextureAsset(ResultRank::getRankTextureName(n.clearRate)).scale(0.1).drawAt(pos + Vec2{ 320, 25 });
			}
		);
	}
}
