#pragma once
#include <memory>

namespace ct
{
	class CourseSelectScene;

	class CourseSelectSceneView
	{
	private:
		class Impl;
		std::shared_ptr<Impl> m_pImpl;
	public:
		CourseSelectSceneView(const CourseSelectScene* const scene);
		~CourseSelectSceneView() = default;

		void update() const;
		void onChangeAction()const;
		void draw()const;
	};
}
