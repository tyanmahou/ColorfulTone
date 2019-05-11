#include "DownloadScene.hpp"

#include"Useful.hpp"
#include"Fade.h"

class DownloadScene::Model
{

};

DownloadScene::DownloadScene():
	m_pModel(std::make_shared<Model>()),
	m_view(this)
{
}

void DownloadScene::init()
{
}

void DownloadScene::update()
{
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
