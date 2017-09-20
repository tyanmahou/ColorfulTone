#include"Tutorial.h"
#include"StartAnime.h"
#include"Fade.h"
#include"PlayKey.h"
#include"AutoPlayManager.h"
#include"ResultRank.h"
#include"FontKinetic.h"
#include"MainScene.h"

//--------------------------------------------------------------------------------
//�֐��F�R���X�g���N�^
//--------------------------------------------------------------------------------
Tutorial::Tutorial() :
	m_timer(0),
	m_font2(20)
{
}

//--------------------------------------------------------------------------------
//�֐��F�f�X�g���N�^
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

	AutoPlayManager::Instance()->m_autoPlay = false;
}
//--------------------------------------------------------------------------------
//�֐��Fupdate
//--------------------------------------------------------------------------------
void Tutorial::update()
{
	m_timer++;


	m_musicGame.update();

	//�^�C�g���ɖ߂�
	if (PlayKey::BigBack().pressedDuration >= 1000|| PlayKey::SmallBack().pressedDuration >= 1000 || m_musicGame.isFinish())
	{
		changeScene(L"title", 3000);
	}
}


//--------------------------------------------------------------------------------
//�֐��Fdraw
//--------------------------------------------------------------------------------

void Tutorial::draw()const
{
	m_musicGame.draw(m_music);

	PutText(L"TutoRial").at(400, 40);

	PutText(L"Press Ese or BackSpace").at(100, 600 - 20);

	if (PlayKey::BigBack().pressed)
		m_font2(L"Ese�������Ŗ߂�").drawCenter(400, 400, ColorF(0, 0.3 + PlayKey::BigBack().pressedDuration*0.70 / 1000));

	if (PlayKey::SmallBack().pressed)
		m_font2(L"BackSpace�������Ŗ߂�").drawCenter(400, 400, ColorF(0, 0.3 + PlayKey::SmallBack().pressedDuration*0.70 / 1000));
}


//--------------------------------------------------------------------------------
//�֐��FdrawFadeIn
//--------------------------------------------------------------------------------
void Tutorial::drawFadeIn(double t) const
{

	draw();
	FadeIn(t, Fade::Default);

}

//--------------------------------------------------------------------------------
//�֐��FdrawFadeOut
//--------------------------------------------------------------------------------
void Tutorial::drawFadeOut(double t) const
{

	draw();
	FadeOut(t, Fade::SmoothCircle);

}