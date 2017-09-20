#pragma once
#include "Game.h"
#include "HighSpeedDemo.h"
#include<future>
class MusicSelectScene :public MyApp::Scene
{
private:
	enum class Mode :unsigned int
	{
		GenreSelect,
		MusicSelect,
		LevelSelect,
	}m_mode;
	int m_timer;				//�^�C�}�[

	Font m_font1;
	Font m_font2;
	Font m_font3;
	Font m_rateFont;

	static unsigned int m_selectMusic;			//�I�𒆂̊y��
	static unsigned int m_selectLevel;			//�I�𒆂̕��ʓ�Փx
	static unsigned int m_selectGenre;			//�I�𒆂̃W������
	static bool m_levelInfoMode;				//�\�����



	mutable Array<MusicData> m_musics;
	HighSpeedDemo m_highSpeedDemo;
	unsigned int m_musicsSize;		//�y�Ȑ�


	bool m_changeMainScene;

	void musicBannerAndInfoDraw()const;
	void genreBannerAndInfoDraw()const;
	void levelSelectDraw()const;
	void highSpeedDraw()const;

	//���o�pEasing
	void EasingStartBySelectMusic();
	void EasingEndBySelectMusic();
	EasingController<int> m_rightShift;
	EasingController<int> m_leftShift;
	EasingController<int> m_bannerSize;
	EasingController<Vec2> m_bannerBGSize;
	EasingController<int> m_bannerBGY;
	EasingController<int> m_infoSeat;
	EasingController<int> m_isMusicDefineEasing;
	EasingController<int> m_isGenreDefineEasing;


public:

	static std::future<void> m_audioResult;


	MusicSelectScene();
	~MusicSelectScene();

	void init() override;
	void update() override;
	void draw() const override;
	void drawFadeIn(double t) const override;
	void drawFadeOut(double t) const override;
};
