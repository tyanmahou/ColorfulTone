#pragma once

#include"ISceneBase.hpp"

class TitleScene :public ISceneBase
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

	void init() override;
	void finally() override;

	void update() override;
	void draw() const override;
	void drawFadeIn(double t) const override;
	void drawFadeOut(double t) const override;
};