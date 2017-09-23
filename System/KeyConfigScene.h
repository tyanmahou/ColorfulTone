#pragma once

#include "Game.h"
#include"Config.h"
#include"KeyConfig.h"


class ConfigScene :public MyApp::Scene
{

private:

	int m_timer = 0;

	ConfigManager m_config;

public:


	ConfigScene();
	~ConfigScene();


	void update() override;
	void draw() const override;

	void drawFadeIn(double t) const override;
	void drawFadeOut(double t) const override;
};