#include <scenes/Scene/Main/MainScene.hpp>
#include <Useful.hpp>
#include <Siv3D.hpp>

namespace ct
{
	//--------------------------------------------------------------------------------
	//関数：コンストラクタ
	//--------------------------------------------------------------------------------
	MainScene::MainScene(const InitData& init) :
		ISceneBase(init)
	{
		auto& data = getData();
		m_musicGame.init(data.m_nowNotes, data.m_scrollRate);

		m_isCourse = data.m_course.isActive();
		if (m_isCourse) {
			m_musicGame.setCourseMode(data.m_resultScore);
		}
	}

	//--------------------------------------------------------------------------------
	//関数：デストラクタ
	//--------------------------------------------------------------------------------
	MainScene::~MainScene()
	{}
	void MainScene::finally()
	{
		m_musicGame.finally();
	}
	void MainScene::updateFadeIn([[maybe_unused]]double t)
	{
		m_musicGame.update();
	}
	//--------------------------------------------------------------------------------
	//関数：update
	//--------------------------------------------------------------------------------
	void MainScene::update()
	{
		m_musicGame.update();

		//****************
		//シーン遷移
		//****************
		if (!m_isCourse && PlayKey::SmallBack().pressedDuration() >= 1000ms || m_musicGame.isFinish()) {
			getData().m_resultScore = m_musicGame.getScore();
			changeScene(SceneName::Result, 2000, CrossFade::No);
		}
		//選曲に戻る
		if (!m_isCourse) {
			if (PlayKey::BigBack().pressedDuration() >= 1000ms) {
				changeScene(SceneName::Select, 2000, CrossFade::No);
			}
		} else {
			if (PlayKey::BigBack().pressedDuration() >= 1000ms) {
				changeScene(SceneName::CourseSelect, 2000, CrossFade::No);
			}
		}

		m_highSpeed.update(getData().m_scrollRate);
		m_musicGame.setScrollRate(getData().m_scrollRate);
	}


	//--------------------------------------------------------------------------------
	//関数：draw
	//--------------------------------------------------------------------------------

	void MainScene::draw()const
	{
		m_musicGame.draw();
		m_musicGame.drawCurrentBPM();
		FontAsset font20(FontName::Regular20);
		const double backAlpha = 0.3 + Min(PlayKey::BigBack().pressedDuration().count() / 1000.0, 0.7);
		const ColorF backColor(0, backAlpha);

		if (!m_isCourse) {
			PutText(U"Press Esc or BackSpace", Arg::center = Vec2{ 100, Scene::Height() - 20 });

			if (PlayKey::BigBack().pressed()) {
				font20(U"Esc長押しで戻る").drawAt(400, 400, backColor);
			}
			if (PlayKey::SmallBack().pressed()) {
				font20(U"BackSpace長押しでリザルトへ").drawAt(400, 400, backColor);
			}
		} else {
			PutText(U"Press Esc", Arg::center = Vec2{ 100, Scene::Height() - 20 });

			if (PlayKey::BigBack().pressed()) {
				font20(U"Esc長押しで諦める").drawAt(400, 400, backColor);
			}

		}
		const MusicData& music = *getData().m_nowNotes.getMusic();
		m_highSpeed.draw(
			music.getMinSoundBeat(),
			music.getMaxSoundBeat(),
			getData().m_scrollRate
		);
	}


	//--------------------------------------------------------------------------------
	//関数：drawFadeIn
	//--------------------------------------------------------------------------------
	void MainScene::drawFadeIn(double t) const
	{
		draw();
		FadeIn(static_cast<FadeFunc_t>(Fade::DrawCanvas), t);
		getData().m_nowNotes.getMusic()->getTexture().resized(350, 350).drawAt(400, 300, ColorF(1, 1 - t));
	}

	//--------------------------------------------------------------------------------
	//関数：drawFadeOut
	//--------------------------------------------------------------------------------
	void MainScene::drawFadeOut(double t) const
	{
		draw();
		FadeOut(static_cast<FadeFunc_t>(Fade::DrawCanvas), t);
	}
}
