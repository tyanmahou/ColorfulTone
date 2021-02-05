#pragma once

#include "ISceneBase.hpp"
#include "DownloadSceneView.hpp"

class DownloadContent;

class DownloadScene : public ISceneBase
{
public:
	struct SelectContent
	{
		uint32 content = 0; // 選択中のコンテンツ
	};
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

	const Array<DownloadContent>& getContents()const;

	int getMoveSelect()const;
	bool isDownloading()const;
	double getProgress()const;

	static SelectContent GetSelectInfo();
};
