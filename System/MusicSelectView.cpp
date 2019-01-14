#include "MusicSelectView.hpp"
#include "MusicSelect.hpp"
#include "Fade.h"
#include <Siv3D.hpp>


MusicSelectView::MusicSelectView(const MusicSelect*const scene):
	m_pScene(scene)
{}


MusicSelectView::~MusicSelectView()
{
}

void MusicSelectView::draw() const
{
	TextureAsset(L"canvasBg").draw();

	auto& musics = m_pScene->getMusics();
	auto selectInfo = MusicSelect::GetSelectInfo();

	auto& music = musics[selectInfo.music];
	// ジャケ絵描画
	Fade::DrawCanvas(m_pScene->getShaderTimer(), [&music]()
	{
		music.getTexture().resize(400, 400).draw(50, 50);
	});
}
