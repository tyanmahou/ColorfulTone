#pragma once 
#include"ISceneBase.hpp"
#include"PlayMusicGame.h"
#include"HighSpeedDemo.h"

class MainScene :public ISceneBase
{
private:
	bool m_isCourse=false;

	PlayMusicGame m_musicGame;
	HighSpeedDemo m_highSpeed;
public:
	MainScene();
	~MainScene();

	void init() override;
	void finally() override;

	void updateFadeIn(double t) override;
	void update() override;
	void draw() const override;
	void drawFadeIn(double t) const override;
	void drawFadeOut(double t) const override;

};


