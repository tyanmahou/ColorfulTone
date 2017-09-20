#pragma once
#include"Game.h"

class CourseSelectScene :public MyApp::Scene
{
private:
	int m_timer = 0;
	Font m_font;
	Font m_levelFont;

	int m_selectCourse=0;

	void musicInfo(int y, int musicID, int notesID)const;
public:
	CourseSelectScene();
	
	void init() override;
	void update() override;
	void draw() const override;
	void drawFadeIn(double t) const override;
	void drawFadeOut(double t) const override;

};
