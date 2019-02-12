#pragma once
#include"Game.h"
#include"HighSpeedDemo.h"
class CoursePlay :public ISceneBase
{
private:
	int m_timer = 0;
	Font m_font1;
	Font m_font2;
	//Font m_levelFont;

	Sound m_sound;
	HighSpeedDemo m_highSpeed;

	void highSpeedDraw()const;
	void musicInfoDraw()const;
//	void musicInfo(int y, int musicID, int notesID)const;
public:
	CoursePlay();

	void init() override;
	void update() override;
	void draw() const override;
	void drawFadeIn(double t) const override;
	void drawFadeOut(double t) const override;

};
