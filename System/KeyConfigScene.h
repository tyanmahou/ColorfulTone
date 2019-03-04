#pragma once

#include"ISceneBase.hpp"
#include"Config.hpp"


class ConfigScene :public ISceneBase
{

private:
	ConfigManager m_config;

public:

	ConfigScene();
	~ConfigScene();

	void update() override;
	void draw() const override;

	void drawFadeIn(double t) const override;
	void drawFadeOut(double t) const override;
};