#pragma once
#include"Game.h"

class CourseSelectScene :public MyApp::Scene
{
private:
	enum class Mode {
		Genre,
		Course
	};
	Mode m_mode = Mode::Genre;

	int m_timer = 0;
	Font m_font;
	Font m_levelFont;

	int m_selectGenre = 0;
	int m_selectCourse = 0;
	Array<CourseData> m_courses;
	void musicInfo(int y, int musicID, int notesID)const;
public:
	CourseSelectScene();

	void init() override;
	void update() override;
	void draw() const override;
	void drawFadeIn(double t) const override;
	void drawFadeOut(double t) const override;

};
