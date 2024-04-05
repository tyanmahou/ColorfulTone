#include <scenes/Scene/Download/DownloadScene.hpp>
#include <Useful.hpp>
#include <core/Data/DownloadContent/DownloadContent.hpp>
#include <core/Data/DownloadContent/Downloader.hpp>
#include <apis/Download/DownloadApi.hpp>
#include <utils/Coro/Fiber/FiberHolder.hpp>
#include <Siv3D.hpp>

namespace
{
	using namespace ct;
	DownloadScene::SelectContent g_selectInfo;
}

namespace ct
{
	class DownloadScene::Model
	{
	private:
		GameData* m_data;

		Array<DownloadContent> m_contents;
		Downloader m_downloader;
		s3d::int32 m_moveSelect = 0;

		bool m_hasNewContent = false;

		Coro::FiberHolder<void> m_loadingTask;
	public:
		Model() {
			m_loadingTask.reset(std::bind(&Model::getListAsync, this));
			m_loadingTask.resume();
		}
		Coro::Fiber<void> getListAsync()
		{
			m_contents = co_await DownloadApi::List();
		}
		void setData(GameData& data)
		{
			m_data = &data;
		}
		bool update()
		{
			if (m_loadingTask.resume()) {
				return false;
			}
			if (m_downloader.isActive()) {
				m_downloader.downloadUpdate();
				return false;
			}

			auto& target = g_selectInfo.content;
			size_t size = m_contents.size();
			m_moveSelect = SharedLogic::MoveSelectV();
			if (m_moveSelect) {
				if (m_moveSelect < 0) {
					++target;
				} else {
					target += static_cast<uint32>(size);
					--target;
				}
				SoundManager::PlaySe(U"select");
			}
			target = size ? target % size : 0;

			// 決定ボタン
			if (PlayKey::Start().down() && size) {
				auto& content = m_contents[g_selectInfo.content];
				bool isDownloaded = content.isDownloaded();
				auto command = System::MessageBoxOKCancel(
					content.getTitle(),
					isDownloaded ?
					U"既にダウンロード済みです。\n再度ダウンロードをしますか？" :
					U"ダウンロードを開始します。"
				);
				if (command == MessageBoxResult::OK) {
					m_downloader.download(content);
					m_hasNewContent = true;
					return false;
				}
			}
			return true;
		}

		const Array<DownloadContent>& getContents() const
		{
			return m_contents;
		}
		s3d::int32 getMoveSelect()const
		{
			return m_moveSelect;
		}
		bool hasNewContent()const
		{
			return m_hasNewContent;
		}
		bool isListLoading() const
		{
			return !m_loadingTask.isDone();
		}
		bool isDownloading()const
		{
			return m_downloader.isActive();
		}
		double getProgress() const
		{
			return m_downloader.getProgress();
		}
	};

	DownloadScene::DownloadScene(const InitData& init) :
		ISceneBase(init),
		m_pModel(std::make_shared<Model>()),
		m_view(this)
	{
		m_pModel->setData(getData());
	}

	void DownloadScene::update()
	{
		if (m_pModel->update()) {
			if (PlayKey::BigBack().down()) {
				if (m_pModel->hasNewContent() &&
					System::MessageBoxYesNo(U"追加ファイルがあります。リロードしますか?") == MessageBoxResult::Yes) {
					this->changeScene(SceneName::Load, 1000);
				} else {
					this->changeScene(SceneName::Title, 1000);
				}
				SoundManager::PlaySe(U"desisionLarge");
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
		if (!m_pModel->isDownloading()) {
			SceneInfo::DrawEsc(s3d::Palette::Black);
			SceneInfo::Draw(U"Enter:決定");
		}
	}

	void DownloadScene::drawFadeIn(double t) const
	{
		FadeIn(Fade::FlipPage, t, [this]() {draw(); }, true);
	}

	void DownloadScene::drawFadeOut([[maybe_unused]]double t) const
	{
		this->draw();
	}

	const Array<DownloadContent>& DownloadScene::getContents() const
	{
		return m_pModel->getContents();
	}

	s3d::int32 DownloadScene::getMoveSelect() const
	{
		return m_pModel->getMoveSelect();
	}

	bool DownloadScene::isListLoading() const
	{
		return m_pModel->isListLoading();
	}

	bool DownloadScene::isDownloading() const
	{
		return m_pModel->isDownloading();
	}

	double DownloadScene::getProgress() const
	{
		return m_pModel->getProgress();
	}

	DownloadScene::SelectContent DownloadScene::GetSelectInfo()
	{
		return g_selectInfo;
	}

}
