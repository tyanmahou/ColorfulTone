#pragma once

#include<memory>

class CourseSelect;

class CourseSelectView
{
private:
	class Impl;
	std::shared_ptr<Impl> m_pImpl;
public:
	CourseSelectView(const CourseSelect*const scene);
	~CourseSelectView() = default;

	void update() const;
	void onChangeAction()const;
	void draw()const;
};

