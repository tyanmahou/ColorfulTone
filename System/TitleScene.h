#pragma once

#include "Game.h"

class TitleScene :public MyApp::Scene
{
public:
	enum class Mode :unsigned int
	{
		GameStart,
		Course,
		KeyConfig,
		Reload,
		Tutorial,
		Download,
		Exit,
	};
private:
	int m_timer;				//タイマー
	Font m_font;

	static Mode m_mode;

	void onEnterMode();
public:


	TitleScene();
	~TitleScene()=default;


	void update() override;
	void draw() const override;
	void drawFadeIn(double t) const override;
	void drawFadeOut(double t) const override;
};