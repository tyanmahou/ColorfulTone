#include "DownloadScene.hpp"

#include"Useful.hpp"
#include"Fade.h"
#include"DownloadContent.hpp"
#include"Downloader.hpp"
#include"DownloadApi.hpp"
#include"Setting.hpp"

namespace
{
	DownloadScene::SelectContent g_selectInfo;
}

class DownloadScene::Model
{
private:
	std::shared_ptr<GameData> m_data;

	Array<DownloadContent> m_contents;
	Downloader m_downloader;
public:
	Model() {
		DownloadApi::List(m_contents);
	}
	void setData(std::shared_ptr<GameData> data)
	{
		m_data = data;
	}
	void update()
	{
	}

	const Array<DownloadContent>& getContents() const
	{
		return m_contents;
	}

};

DownloadScene::DownloadScene():
	m_pModel(std::make_shared<Model>()),
	m_view(this)
{
}

void DownloadScene::init()
{
	m_pModel->setData(m_data);
}

void DownloadScene::update()
{
	m_pModel->update();
	m_view.update();
}

void DownloadScene::finally()
{
}

void DownloadScene::draw() const
{
	m_view.draw();
	// シーン情報
	SceneInfo::Draw(L"Enter:決定 Esc:タイトル戻る");
}

void DownloadScene::drawFadeIn(double t) const
{
	FadeIn(Fade::FlipPage, t, [this]() {draw(); }, true);
}

void DownloadScene::drawFadeOut(double t) const
{
	this->draw();
}

const Array<DownloadContent>& DownloadScene::getContents() const
{
	return m_pModel->getContents();
}

int DownloadScene::getMoveSelect() const
{
	return 0;
}

DownloadScene::SelectContent DownloadScene::GetSelectInfo()
{
	return g_selectInfo;
}
