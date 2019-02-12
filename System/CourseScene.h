#pragma once
#include"Game.h"

class CourseSelectScene :public ISceneBase
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

	static int m_selectGenre;
	static int m_selectCourse;
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
