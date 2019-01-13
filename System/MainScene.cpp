#include"MainScene.h"
#include"Fade.h"
#include"PlayKey.h"
#include"AutoPlayManager.h"
#include"ResultRank.h"
#include"FontKinetic.h"
#include"StartAnime.h"

//--------------------------------------------------------------------------------
//関数：コンストラクタ
//--------------------------------------------------------------------------------
MainScene::MainScene() :
	m_timer(0),
	m_font2(20)
{
}

//--------------------------------------------------------------------------------
//関数：デストラクタ
//--------------------------------------------------------------------------------
MainScene::~MainScene()
{

}
void MainScene::init()
{
	auto& nowMusic = m_data->m_nowMusics;

	m_musicGame.init(nowMusic, m_data->m_selectLevel, m_data->m_scrollRate);

	m_isCourse = m_data->m_isCoursePlay;
	if (m_isCourse)
		m_musicGame.setCourseMode(m_data->m_life);
}
//--------------------------------------------------------------------------------
//関数：update
//--------------------------------------------------------------------------------
void MainScene::update()
{
	m_timer++;

	m_musicGame.update();

	//****************
	//シーン遷移
	//****************
	if (!m_data->m_isCoursePlay&&PlayKey::SmallBack().pressedDuration >= 1000 || m_musicGame.isFinish())
	{
		m_data->m_resultScore = m_musicGame.getScore();
		if (m_isCourse)
			m_data->m_life = m_musicGame.getLife();
		changeScene(L"result", 3000);
	}
	//選曲に戻る
	if (!m_isCourse)
	{
		if (PlayKey::BigBack().pressedDuration >= 1000)
		{
			changeScene(L"select", 3000);
		}
	}
	else
	{
		if (PlayKey::BigBack().pressedDuration >= 1000)
		{
			m_data->m_isCoursePlay = false;
			changeScene(L"courseSelect", 3000);
		}
	}

	m_highSpeed.update(m_data->m_scrollRate);
	m_musicGame.setScrollRate(m_data->m_scrollRate);

}


//--------------------------------------------------------------------------------
//関数：draw
//--------------------------------------------------------------------------------

void MainScene::draw()const
{

	m_musicGame.draw(m_data->m_nowMusics);
	m_musicGame.drawCurrentBPM();


	if (!m_isCourse)
	{
		PutText(L"Press Esc or BackSpace").at(100, Window::Height() - 20);

		if (PlayKey::BigBack().pressed)
			m_font2(L"Esc長押しで戻る").drawCenter(400, 400, ColorF(0, 0.3 + PlayKey::BigBack().pressedDuration*0.70 / 1000));

		if (PlayKey::SmallBack().pressed)
			m_font2(L"BackSpace長押しでリザルトへ").drawCenter(400, 400, ColorF(0, 0.3 + PlayKey::SmallBack().pressedDuration*0.70 / 1000));
	}
	else
	{
		PutText(L"Press Esc").at(100, Window::Height() - 20);

		if (PlayKey::BigBack().pressed)
			m_font2(L"Esc長押しで諦める").drawCenter(400, 400, ColorF(0, 0.3 + PlayKey::BigBack().pressedDuration*0.70 / 1000));

	}
	m_highSpeed.draw(
		m_data->m_nowMusics.getMinSoundBeat(),
		m_data->m_nowMusics.getMaxSoundBeat(),
		m_data->m_scrollRate);
}


//--------------------------------------------------------------------------------
//関数：drawFadeIn
//--------------------------------------------------------------------------------
void MainScene::drawFadeIn(double t) const
{

	draw();
	FadeIn(Fade::Default, t);
	m_data->m_nowMusics.getTexture().resize(300, 300).drawAt(400, 300, ColorF(1, 1 - t));

}

//--------------------------------------------------------------------------------
//関数：drawFadeOut
//--------------------------------------------------------------------------------
void MainScene::drawFadeOut(double t) const
{

	draw();
	FadeOut(Fade::SmoothCircle, t);

}