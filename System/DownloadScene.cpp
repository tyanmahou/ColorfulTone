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
	// �V�[�����
	SceneInfo::Draw(L"Enter:���� Esc:�^�C�g���߂�");
}

void DownloadScene::drawFadeIn(double t) const
{
	FadeIn(Fade::FlipPage, t, [this]() {draw(); }, true);
}

void DownloadScene::drawFadeOut(double t) const
{
	this->draw();
}
