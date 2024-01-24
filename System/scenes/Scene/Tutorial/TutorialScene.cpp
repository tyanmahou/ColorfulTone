#include <scenes/Scene/Tutorial/TutorialScene.hpp>
#include <Useful.hpp>
#include <core/Input/AutoPlayManager.hpp>
#include <scenes/utils/SharedDraw.hpp>
#include <Siv3D.hpp>

namespace ct
{
	//--------------------------------------------------------------------------------
	//関数：コンストラクタ
	//--------------------------------------------------------------------------------
	TutorialScene::TutorialScene(const InitData& init) :
		ISceneBase(init),
		m_music(U"Sample", U"Sample/tutorial/", U"Sample/tutorial/tutorial.ini")
	{

		m_musicGame.init(m_music[0], getData().m_scrollRate);

		AutoPlayManager::SetAutoPlay(false);
	}

	//--------------------------------------------------------------------------------
	//関数：デストラクタ
	//--------------------------------------------------------------------------------
	TutorialScene::~TutorialScene()
	{

	}
	void TutorialScene::finally()
	{
		m_musicGame.finally();
	}
	void TutorialScene::updateFadeIn([[maybe_unused]]double t)
	{
		m_musicGame.update();
	}
	//--------------------------------------------------------------------------------
	//関数：update
	//--------------------------------------------------------------------------------
	void TutorialScene::update()
	{
		m_musicGame.update();

		//タイトルに戻る
		if (PlayKey::BigBack().pressedDuration() >= 1000ms || PlayKey::SmallBack().pressedDuration() >= 1000ms || m_musicGame.isFinish()) {
			changeScene(SceneName::Title, 2000, CrossFade::No);
		}
		m_highSpeed.update(getData().m_scrollRate);
		m_musicGame.setScrollRate(getData().m_scrollRate);
	}


	//--------------------------------------------------------------------------------
	//関数：draw
	//--------------------------------------------------------------------------------

	void TutorialScene::draw()const
	{
		m_musicGame.draw();

		PutText(U"Tutorial", Arg::center = Vec2{ Scene::Center().x, 40 });

		PutText(U"Press Ese or BackSpace", Arg::center = Vec2{ 100, Scene::Height() - 20 });
		SharedDraw::LongPressBack(PlayKey::BigBack(), U"Esc長押しで戻る");
		SharedDraw::LongPressBack(PlayKey::SmallBack(), U"BackSpace長押しで戻る");

		SharedDraw::HighSpeedPlay(
			m_highSpeed,
			m_music,
			getData().m_scrollRate
		);
	}


	//--------------------------------------------------------------------------------
	//関数：drawFadeIn
	//--------------------------------------------------------------------------------
	void TutorialScene::drawFadeIn(double t) const
	{
		this->draw();
		FadeIn(static_cast<FadeFunc_t>(Fade::DrawCanvas), t);
	}

	//--------------------------------------------------------------------------------
	//関数：drawFadeOut
	//--------------------------------------------------------------------------------
	void TutorialScene::drawFadeOut(double t) const
	{
		this->draw();
		FadeOut(static_cast<FadeFunc_t>(Fade::DrawCanvas), t);
	}
}
