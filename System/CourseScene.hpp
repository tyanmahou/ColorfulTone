#pragma once
#include"ISceneBase.hpp"
#include"CourseSceneView.hpp"

class HighSpeedDemo;

class CourseScene :public ISceneBase
{
private:
	class Model;
	std::shared_ptr<Model> m_pModel;
	CourseSceneView m_view;
public:
	CourseScene();

	void init() override;
	void update() override;
	void finally() override;
	void draw() const override;
	void drawFadeIn(double t) const override;
	void drawFadeOut(double t) const override;

	const PlayCourse& getPlay() const;
	const HighSpeedDemo& getHighSpeedDemo()const;
	float getScrollRate()const
	{
		return m_data->m_scrollRate;
	}
};
