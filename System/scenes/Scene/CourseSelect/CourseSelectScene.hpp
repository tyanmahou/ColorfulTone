#pragma once
#include <scenes/Scene/ISceneBase.hpp>
#include <scenes/Scene/CourseSelect/CourseSelectSceneView.hpp>

namespace ct
{
	class CourseSelectScene : public ISceneBase
	{
	public:
		enum class Action : s3d::uint8
		{
			GenreSelect,
			CourseSelect,
		};
		struct SelectCourseInfo
		{
			s3d::uint32 genre = 0;	// 選択中のジャンル
			s3d::uint32 course = 0;	// 選択中のコース
		};
	private:

		class Model;
		std::shared_ptr<Model> m_pModel;
		CourseSelectSceneView m_view;
	public:
		CourseSelectScene(const InitData& init);
		~CourseSelectScene() = default;

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
		s3d::int32 getMoveSelect() const;
	};
}
