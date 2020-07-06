#include "CourseSceneView.hpp"
#include "CourseScene.hpp"
#include "SharedDraw.hpp"
#include "Useful.hpp"
#include "EasingSequence.hpp"
namespace
{
}

class CourseSceneView::Impl
{
private:

	const CourseScene*const m_pScene;
	SharedDraw::DrawBGLight m_lights;
	EasingSequence m_timers;
public:
	Impl(const CourseScene*const scene) :
		m_pScene(scene)
	{
		m_timers.regist(L"label", { 0,1,Easing::Circ, 1000 });
		m_timers.regist(L"memo", { 0,1,Easing::Back, 1000 });
	}

	void update()
	{
		m_timers.update();
		m_lights.update();
	}

	void draw() const
	{
		TextureAsset(L"canvasBg").draw();
		m_lights.draw();

		const auto& playing = m_pScene->getPlay();
		const auto& notes = playing.getCurrentNotes();
		const auto& music = *notes.getMusic();

		const auto animeTime = m_timers[L"label"].easeOut();
		// ジャケ絵描画
		const Vec2 pos{ Constants::JacketCenter, 250 };
		const Vec2 size{ 310,310 };
		RectF(pos - size / 2.0, size).draw(notes.getColor());
		music
			.getTexture()
			.resize(size)
			.rotate(Math::Radians(-7.0))
			.drawAt(Constants::JacketCenter, 250);
		if (music.isFavorite()) {
			TextureAsset(L"favorite").drawAt(pos + Vec2{ 155, -155 });
		}
		// 曲情報
		SharedDraw::JacketInfo infoView;
		infoView
			.drawLabel(music.getTexture(), animeTime)
			.drawLabel()
			.drawLine()
			.drawTitle(music.getMusicName())
			.drawSub(notes.getLevelNameAndLevel())
			.drawDetailRight(music.getFormattedBpm());

		// track
		const auto& musics = Game::Musics();
		SharedDraw::Select<CourseData::Data>()
			.setLoop(false)
			.setOffset(-30.0)
			.setDrawble([&](const CourseData::Data& d, Vec2 pos) {
			musics[d.first].getTexture().resize(50, 50).drawAt(pos + Vec2{ 37, 30 });
		}).draw(
			playing.getCourse().getNotesIDs(),
			playing.getTrackIndex(),
			[&](const CourseData::Data& d)->decltype(auto) {return musics[d.first].getMusicName(); }
		);

		// 譜面情報
		SharedDraw::MemoInfo memoInfo;
		memoInfo
			.setPos(SharedDraw::MemoInfo::DefaultPos + Vec2{ 0, 300 * (1.0 - m_timers[L"memo"].easeOut()) });
		if(CourseScene::GetMemoInfo()== CourseScene::MemoInfo::Course)
		{
			memoInfo.draw(playing.getScore(), false);
		}
		else {
			memoInfo.draw(notes);
		}
		// 付箋
		String track = L"Track " + Format(playing.getTrackOrder());
		SharedDraw::Sticky(
			&playing.getCourse().getTitle(),
			&track
		);
		// ハイスピ
		SharedDraw::HighSpeed(
			m_pScene->getHighSpeedDemo(),
			music,
			m_pScene->getScrollRate()
		);
	}
};

CourseSceneView::CourseSceneView(const CourseScene * const scene) :
	m_pImpl(std::make_shared<Impl>(scene))
{}

void CourseSceneView::update() const
{
	m_pImpl->update();
}

void CourseSceneView::draw() const
{
	m_pImpl->draw();
}