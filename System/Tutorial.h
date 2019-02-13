#pragma once

#include"ISceneBase.hpp"
#include"PlayMusicGame.h"
class Tutorial :public ISceneBase
{
private:

	int m_timer;				//タイマー
	Font m_font2;

	MusicData m_music;
	PlayMusicGame m_musicGame;

public:

	Tutorial();
	~Tutorial();

	void init() override;
	void update() override;
	void draw() const override;
	void drawFadeIn(double t) const override;
	void drawFadeOut(double t) const override;

};



