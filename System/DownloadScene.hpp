#pragma once

#include "ISceneBase.hpp"
#include "DownloadSceneView.hpp"

class DownloadScene : public ISceneBase
{
private:
	class Model;
	std::shared_ptr<Model> m_pModel;
	DownloadSceneView m_view;
public:
	DownloadScene();
	~DownloadScene() = default;

	void init() override;
	void update() override;
	void finally() override;
	void draw() const override;
	void drawFadeIn(double t) const override;
	void drawFadeOut(double t) const override;
};
