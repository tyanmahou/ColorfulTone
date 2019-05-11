#include "DownloadScene.hpp"

#include"Useful.hpp"
#include"Fade.h"
#include"DownloadContent.hpp"
#include"DownloadApi.hpp"

class DownloadScene::Model
{
private:
	Array<DownloadContent> m_contents;
public:
	Model() {
		DownloadApi::List(m_contents);
		Println(m_contents[0].getTitle());
	}
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
