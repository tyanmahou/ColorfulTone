#pragma once

#include"ISceneBase.hpp"
#include"TitleSceneView.hpp"
class TitleScene :public ISceneBase
{
public:
	enum class Mode :unsigned int
	{
		GameStart,
		Course,
		KeyConfig,
		Download,
		Reload,
		Tutorial,
		Access,
		Exit,
	};
private:
	TitleSceneView m_view;
	static Mode m_mode;

	void onEnterMode();
public:


	TitleScene();
	~TitleScene()=default;

	void init() override;
	void finally() override;

	void update() override;
	void draw() const override;
	void drawFadeIn(double t) const override;
	void drawFadeOut(double t) const override;
	static Mode GetMode();
};