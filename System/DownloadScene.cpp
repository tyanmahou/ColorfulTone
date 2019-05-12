#include "DownloadScene.hpp"

#include"Useful.hpp"
#include"Fade.h"
#include"DownloadContent.hpp"
#include"Downloader.hpp"
#include"DownloadApi.hpp"
#include"Setting.hpp"
#include"SharedLogic.hpp"
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
	int m_moveSelect = 0;
public:
	Model() {
		DownloadApi::List(m_contents);
	}
	void setData(std::shared_ptr<GameData> data)
	{
		m_data = data;
	}
	bool update()
	{
		auto& target = g_selectInfo.content;
		size_t size = m_contents.size();
		m_moveSelect = SharedLogic::MoveSelect();
		if (m_moveSelect)
		{
			if (m_moveSelect < 0)
			{
				++target;
			}
			else
			{
				target += size;
				--target;
			}
			SoundManager::SE::Play(L"select");

		}
		target = size ? target % size : 0;

		if (m_downloader.isActive()) {
			m_downloader.downloadUpdate();
			return false;
		}
		// 決定ボタン
		if (PlayKey::Start().clicked && size)
		{
			auto& content = m_contents[g_selectInfo.content];
			auto command = MessageBox::Show(
				content.getTitle(),
				content.isDownloaded()?
				L"既にダウンロード済みです。\n再度ダウンロードをしますか？":
				L"ダウンロードを開始します。", 
				MessageBoxStyle::OkCancel
			);
			if (command == MessageBoxCommand::Ok)
			{
				m_downloader.download(content);
				return false;
			}
		}
		return true;
	}

	const Array<DownloadContent>& getContents() const
	{
		return m_contents;
	}
	int getMoveSelect()const
	{
		return m_moveSelect;
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
	if (m_pModel->update())
	{
		if (PlayKey::BigBack().clicked)
		{
			this->changeScene(L"title", 1000);
			SoundManager::SE::Play(L"cancel");
		}
	}
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
	return m_pModel->getMoveSelect();
}

DownloadScene::SelectContent DownloadScene::GetSelectInfo()
{
	return g_selectInfo;
}
