﻿#pragma once
#include "Game.h"
#include "FileLoadView.hpp"

class FileLoad :public MyApp::Scene
{
private:
	int m_timer;			//タイマー
	enum class State {
		Loading,
		OnLoadCompleted,
		Wait
	} m_state = State::Loading;
	FileLoadView m_view;	//ビュー
public:

	FileLoad();
	~FileLoad();

	void init()override;
	void update() override;
	void draw() const override;
	void drawFadeIn(double t) const override;
	void drawFadeOut(double t) const override;
};
