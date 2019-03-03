#include"MainScene.h"
#include "Useful.hpp"
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
	m_musicGame.init(m_data->m_nowNotes, m_data->m_scrollRate);

	m_isCourse = m_data->m_course.isActive();
	if (m_isCourse)
	{
		m_musicGame.setCourseMode(m_data->m_resultScore);
	}
}
void MainScene::finally()
{
	m_musicGame.finally();
}
void MainScene::updateFadeIn(double t)
{
	m_musicGame.update();
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
	if (!m_isCourse&&PlayKey::SmallBack().pressedDuration >= 1000 || m_musicGame.isFinish())
	{
		m_data->m_resultScore = m_musicGame.getScore();
		changeScene(SceneName::Result, 2000, false);
	}
	//選曲に戻る
	if (!m_isCourse)
	{
		if (PlayKey::BigBack().pressedDuration >= 1000)
		{
			changeScene(SceneName::Select, 2000, false);
		}
	}
	else
	{
		if (PlayKey::BigBack().pressedDuration >= 1000)
		{
			changeScene(SceneName::CourseSelect, 2000, false);
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
	m_musicGame.draw();
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
	const MusicData & music = *m_data->m_nowNotes.getMusic();
	m_highSpeed.draw(
		music.getMinSoundBeat(),
		music.getMaxSoundBeat(),
		m_data->m_scrollRate
	);
}


//--------------------------------------------------------------------------------
//関数：drawFadeIn
//--------------------------------------------------------------------------------
void MainScene::drawFadeIn(double t) const
{
	draw();
	FadeIn(static_cast<FadeFunc_t>(Fade::DrawCanvas), t);
	m_data->m_nowNotes.getMusic()->getTexture().resize(350, 350).drawAt(400, 300, ColorF(1, 1 - t));
}

//--------------------------------------------------------------------------------
//関数：drawFadeOut
//--------------------------------------------------------------------------------
void MainScene::drawFadeOut(double t) const
{
	draw();
	FadeOut(static_cast<FadeFunc_t>(Fade::DrawCanvas), t);
}