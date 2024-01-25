#include <core/Data/DownloadContent/Downloader.hpp>
#include <core/Data/DownloadContent/DownloadContent.hpp>
#include <core/Data/DownloadContent/DownloadHistory.hpp>
#include <commons/Secret/Secret_Product.hpp>
#include <utils/Coro/Fiber/FiberHolder.hpp>
#include <utils/Coro/AsyncGenerator/AsyncGenerator.hpp>
#include <utils/Thread/Task.hpp>
#include <Siv3D.hpp>

namespace
{
	struct ScopedTmpFile
	{
		ScopedTmpFile() :
			path(FileSystem::UniqueFilePath())
		{}
		~ScopedTmpFile()
		{
			FileSystem::Remove(path);
		}
		FilePath path;
	};
}
namespace ct
{
	class Downloader::Impl
	{
	public:
		bool download(DownloadContent& content)
		{
			if (!Network::IsConnected()) {
				return false;
			}
			m_progress = 0;
			m_pContent = &content;
			m_downloader.reset(std::bind(&Impl::donwloadAsync, this));
			return true;
		}
		bool downloadUpdate()
		{
			return m_downloader.resume();
		}
		bool isActive()
		{
			return  !m_downloader.isDone();
		}
		double getProgress()
		{
			return m_downloader.isDone() ? 1.0 : m_progress;
		}
		Coro::Fiber<> donwloadAsync()
		{
			ScopedTmpFile scopedTmpFile;
			m_progress = 0;
			co_await downloadInternal(scopedTmpFile.path).each([this](double progress){
				m_progress = progress;
			});
			m_progress = 1;
		}
		Coro::AsyncGenerator<double> downloadInternal(FilePathView tmpPath)
		{
			AsyncHTTPTask loadTask = SimpleHTTP::SaveAsync(Secret::DOWNLOAD_URL + m_pContent->getDownloadUrl(), tmpPath);
			while (!loadTask.isReady()) {
				HTTPProgress progress = loadTask.getProgress();
				if (progress.download_total_bytes) {
					co_yield static_cast<double>(progress.downloaded_bytes) / static_cast<double>(*progress.download_total_bytes);
				} else {
					co_yield 0.0;
				}
				co_await Coro::Yield{};
			}
			if (!loadTask.isSucceeded()) {
				co_return;
			}
			// Zip解凍
			co_await Thread::Task([&] {
				ZIPReader zipReader(tmpPath);
				zipReader.extractAll(m_pContent->getSaveLocalPath());
				m_pContent->setDownloaded(true);
				bool isUpdate = DownloadHistory::Add(m_pContent->getDownloadId());
				if (isUpdate) {
					DownloadHistory::Save();
				}
			});
		}
	private:
		Coro::FiberHolder<void> m_downloader;
		double m_progress = 0;
		DownloadContent* m_pContent;

	};
	Downloader::Downloader() :
		m_pImpl(std::make_shared<Impl>())
	{
	}
	bool Downloader::isActive() const
	{
		return m_pImpl->isActive();
	}

	bool Downloader::downloadUpdate()const
	{
		return m_pImpl->downloadUpdate();
	}

	bool Downloader::download(DownloadContent& content)const
	{
		return m_pImpl->download(content);
	}

	double Downloader::getProgress() const
	{
		return m_pImpl->getProgress();
	}
}
