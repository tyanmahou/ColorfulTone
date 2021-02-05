#pragma once

#include"ISceneBase.hpp"
#include"ConfigMain.hpp"


class ConfigScene :public ISceneBase
{

private:
	ConfigMain m_config;

public:

	ConfigScene();
	~ConfigScene();

	void update() override;
	void draw() const override;

	void drawFadeIn(double t) const override;
	void drawFadeOut(double t) const override;
};