#include "ResultSceneView.hpp"
#include "ResultScene.hpp"
#include "Useful.hpp"
#include "SharedDraw.hpp"

namespace
{
	void  DrawMusicInfo(const NotesData& notes)
	{
		const MusicData& music = *notes.getMusic();
		music
			.getTexture()
			.resize(Constants::JacketWidth, Constants::JacketWidth)
			.drawAt(Constants::JacketCenter, 250);

		SharedDraw::JacketInfo infoView;
		infoView
			.drawLine()
			.drawTitle(music.getMusicName())
			.drawSub(music.getArtistAndAuthority())
			.drawDetail(notes.getLevelName()+ L" Lv" + Format(notes.getLevel()), notes.getColor());
	}
}

class ResultSceneView::Impl
{
private:
	const ResultScene* const m_pScene;
	SharedDraw::DrawBGLight m_lights;

public:
	Impl(const ResultScene* const scene) :
		m_pScene(scene)
	{}

	void update()
	{
		m_lights.update();
	}

	void draw()const
	{
		// BG
		TextureAsset(L"canvasBg").draw();
		m_lights.draw();

		const NotesData& notes = m_pScene->getNotes();
		::DrawMusicInfo(notes);

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

void ResultSceneView::update() const
{
	m_impl->update();
}

void ResultSceneView::draw() const
{
	m_impl->draw();
}
