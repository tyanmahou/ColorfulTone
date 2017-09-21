#pragma once
#include "Game.h"
#include<future>
#include<mutex>


class FileLoad :public MyApp::Scene
{
private:
	int m_timer;				//タイマー
	Font m_font;
public:

	static std::future<void> m_loadResult;

	FileLoad();
	~FileLoad();

	void init()override;
	void update() override;
	void draw() const override;
	void drawFadeIn(double t) const override;
	void drawFadeOut(double t) const override;
};

std::mutex& GetMutex();
