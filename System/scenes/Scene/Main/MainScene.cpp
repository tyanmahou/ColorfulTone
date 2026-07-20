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
		m_musicGame.init(session().getNotes(), data.m_scrollRate);

		if (session().keepScore()) {
			m_musicGame.setCourseMode(session().getScore());
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
		if (session().canForceFinishMusicGame() && PlayKey::SmallBack().pressedDuration() >= 1000ms || m_musicGame.isFinish()) {
			session().updateScore(m_musicGame.getScore());
			changeScene(SceneName::Result, 2000, CrossFade::No);
		}
		//選曲に戻る
		if (PlayKey::BigBack().pressedDuration() >= 1000ms) {
			changeScene(session().selectScene(), 2000, CrossFade::No);
			getData().session.exit();
		}
		double& scrollRate = getData().m_scrollRate;
		m_highSpeed.update(scrollRate);
		m_musicGame.setScrollRate(scrollRate);
	}


	//--------------------------------------------------------------------------------
	//関数：draw
	//--------------------------------------------------------------------------------

	void MainScene::draw()const
	{
		m_musicGame.draw();

		{
			bool canForceFinish = session().canForceFinishMusicGame();
			PutText(
				canForceFinish ? U"Press Esc or BackSpace" : U"Press Esc",
				Arg::center = Vec2{ 100, Scene::Height() - 20 }
			);
			SharedDraw::LongPressBack(
				PlayKey::BigBack(),
				canForceFinish ? U"Esc長押しで戻る" : U"Esc長押しで諦める"
			);
			if (canForceFinish) {
				SharedDraw::LongPressBack(PlayKey::SmallBack(), U"BackSpace長押しでリザルトへ");
			}
		}
		SharedDraw::HighSpeedPlay(
			m_highSpeed,
			m_musicGame.getPlayNotesData(),
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
		session().getMusic().getTexture().resized(350, 350).drawAt(400, 300, ColorF(1, 1 - t));
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
