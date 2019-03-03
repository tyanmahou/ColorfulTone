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
		m_pScene(scene),
		m_timers({
			{0, 1.0, Easing::Circ, 1000},
			})
	{}

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

		const auto animeTime = m_timers[0].easeOut();
		// ジャケ絵描画
		const Vec2 pos{ Constants::JacketCenter, 250 };
		const Vec2 size{ 310,310 };
		RectF(pos - size / 2.0, size).draw(notes.getColor());
		music
			.getTexture()
			.resize(size)
			.rotate(Math::Radians(-7.0))
			.drawAt(Constants::JacketCenter, 250);
		// 曲情報
		SharedDraw::JacketInfo infoView;
		infoView
			.drawLabel(music.getTexture(), animeTime)
			.drawLabel()
			.drawLine()
			.drawTitle(music.getMusicName())
			.drawSub(notes.getLevelNameAndLevel())
			.drawDetailRight(music.getFormattedBpm());

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
