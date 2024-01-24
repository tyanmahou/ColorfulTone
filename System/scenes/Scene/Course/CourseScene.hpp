#pragma once
#include <scenes/Scene/ISceneBase.hpp>
#include <scenes/Scene/Course/CourseSceneView.hpp>

namespace ct
{
	class HighSpeedDemo;
	class ConfigMain;

	class CourseScene :public ISceneBase
	{
	public:
		enum class MemoInfo : bool
		{
			Course,
			Notes,
		};
	private:
		class Model;
		std::shared_ptr<Model> m_pModel;
		CourseSceneView m_view;
	public:
		CourseScene(const InitData& init);

		void update() override;
		void finally() override;
		void draw() const override;
		void drawFadeIn(double t) const override;
		void drawFadeOut(double t) const override;

		const PlayCourse& getPlay() const;
		const HighSpeedDemo& getHighSpeedDemo()const;
		const ConfigMain& getConfig()const;

		double getScrollRate()const
		{
			return getData().m_scrollRate;
		}

		static MemoInfo GetMemoInfo();
	};
}
