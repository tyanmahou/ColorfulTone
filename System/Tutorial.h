#pragma once
#include"PlayMusicGame.h"
class Tutorial :public MyApp::Scene
{
private:

	int m_timer;				//�^�C�}�[
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



