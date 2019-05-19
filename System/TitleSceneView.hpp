#pragma once
#include<memory>

class TitleScene;

class TitleSceneView
{
private:
	class Impl;
	std::shared_ptr<Impl> m_pImpl;
public:
	TitleSceneView(const TitleScene* const scene);
	void draw()const;
};