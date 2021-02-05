#pragma once
#include<memory>
class CourseScene;

class CourseSceneView
{
private:
	class Impl;
	std::shared_ptr<Impl> m_pImpl;
public:
	CourseSceneView(const CourseScene* const scene);

	~CourseSceneView() = default;

	void update() const;
	void draw()const;
};

