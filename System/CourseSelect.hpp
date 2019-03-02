#pragma once
#include "ISceneBase.hpp"
#include "CourseSelectView.hpp"

class CourseSelect : public ISceneBase
{
public:
	enum class Action : uint8
	{
		GenreSelect,
		CourseSelect,
	};
	struct SelectCourseInfo
	{
		uint32 genre = 0;	// 選択中のジャンル
		uint32 course = 0;	// 選択中のコース
	};
private:

	class Model;
	std::shared_ptr<Model> m_pModel;
	CourseSelectView m_view;
public:
	CourseSelect();
	~CourseSelect() = default;

	void init() override;
	void update() override;
	void finally() override;
	void draw() const override;
	void drawFadeIn(double t) const override;
	void drawFadeOut(double t) const override;

	static SelectCourseInfo GetSelectInfo();

	const Array<CourseData>& getCourses()const;

	Action getAction()const;

	// previous , current
	std::pair<Action, Action> getChangeAction()const;
	int getMoveSelect()const;
};

