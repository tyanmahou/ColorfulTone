#pragma once 
#include"PlayMusicGame.h"

class MainScene :public MyApp::Scene
{
private:
	bool m_isCourse=false;

	int m_timer;				//タイマー

	Font m_font2;

	PlayMusicGame m_musicGame;
public:
	MainScene();
	~MainScene();

	void init() override;
	void update() override;
	void draw() const override;
	void drawFadeIn(double t) const override;
	void drawFadeOut(double t) const override;

};


