#pragma once
#include "Game.h"

class FileLoad :public MyApp::Scene
{
private:
	int m_timer;				//タイマー
	Font m_font;
public:

	FileLoad();
	~FileLoad();

	void init()override;
	void update() override;
	void draw() const override;
	void drawFadeIn(double t) const override;
	void drawFadeOut(double t) const override;
};
