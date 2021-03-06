#pragma once

#include"ISceneBase.hpp"
#include"PlayMusicGame.h"
#include"MusicData.h"
class Tutorial :public ISceneBase
{
private:

	MusicData m_music;
	PlayMusicGame m_musicGame;

public:

	Tutorial();
	~Tutorial();

	void init() override;
	void finally() override;
	void update() override;
	void draw() const override;
	void drawFadeIn(double t) const override;
	void drawFadeOut(double t) const override;

};



