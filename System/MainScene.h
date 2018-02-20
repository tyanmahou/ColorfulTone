#pragma once 
#include"PlayMusicGame.h"
#include"HighSpeedDemo.h"
class MainScene :public MyApp::Scene
{
private:
	bool m_isCourse=false;

	int m_timer;				//タイマー

	Font m_font2;

	PlayMusicGame m_musicGame;
	HighSpeedDemo m_highSpeed;
public:
	MainScene();
	~MainScene();

	void init() override;
	void update() override;
	void draw() const override;
	void drawFadeIn(double t) const override;
	void drawFadeOut(double t) const override;

};


