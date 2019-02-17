#include"Tutorial.h"
#include "Useful.hpp"

#include"StartAnime.h"
#include"AutoPlayManager.h"
#include"ResultRank.h"
#include"FontKinetic.h"
#include"MainScene.h"

//--------------------------------------------------------------------------------
//関数：コンストラクタ
//--------------------------------------------------------------------------------
Tutorial::Tutorial() :
	m_timer(0),
	m_font2(20)
{
}

//--------------------------------------------------------------------------------
//関数：デストラクタ
//--------------------------------------------------------------------------------
Tutorial::~Tutorial()
{

}
void Tutorial::init()
{
	SoundAsset(L"title").stop();
	SoundAsset::Release(L"title");
	
	m_music = MusicData(L"Sample", L"Sample/tutorial/", L"Sample/tutorial/tutorial.ini");

	m_musicGame.init(m_music, 0, m_data->m_scrollRate);

	AutoPlayManager::SetAutoPlay(false);
}
void Tutorial::finally()
{
	m_musicGame.finally();
}
//--------------------------------------------------------------------------------
//関数：update
//--------------------------------------------------------------------------------
void Tutorial::update()
{
	m_timer++;


	m_musicGame.update();

	//タイトルに戻る
	if (PlayKey::BigBack().pressedDuration >= 1000|| PlayKey::SmallBack().pressedDuration >= 1000 || m_musicGame.isFinish())
	{
		changeScene(L"title", 2000, false);
	}
}


//--------------------------------------------------------------------------------
//関数：draw
//--------------------------------------------------------------------------------

void Tutorial::draw()const
{
	m_musicGame.draw(m_music);

	PutText(L"Tutorial").at(Window::Center().x, 40);

	PutText(L"Press Ese or BackSpace").at(100, Window::Height() - 20);

	if (PlayKey::BigBack().pressed)
		m_font2(L"Ese長押しで戻る").drawCenter(400, 400, ColorF(0, 0.3 + PlayKey::BigBack().pressedDuration*0.70 / 1000));

	if (PlayKey::SmallBack().pressed)
		m_font2(L"BackSpace長押しで戻る").drawCenter(400, 400, ColorF(0, 0.3 + PlayKey::SmallBack().pressedDuration*0.70 / 1000));
}


//--------------------------------------------------------------------------------
//関数：drawFadeIn
//--------------------------------------------------------------------------------
void Tutorial::drawFadeIn(double t) const
{
	this->draw();
	FadeIn(static_cast<void(*)(double, const Color&)>(Fade::DrawCanvas), t, Palette::White);
}

//--------------------------------------------------------------------------------
//関数：drawFadeOut
//--------------------------------------------------------------------------------
void Tutorial::drawFadeOut(double t) const
{
	this->draw();
	FadeOut(static_cast<void(*)(double, const Color&)>(Fade::DrawCanvas), t, Palette::White);
}