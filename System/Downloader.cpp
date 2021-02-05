#include "Downloader.hpp"
#include "DownloadContent.hpp"
#include "DownloadHistory.hpp"
#include "Setting.hpp"
#include "MultiThread.hpp"
#include<Siv3D.hpp>

class Downloader::Impl
{
private:
	HTTPClient m_client;
	ByteArray m_byte;
	DownloadContent* m_pContent;
	bool m_isActive;
	bool m_isSaving = false;

public:
	bool save()
	{
		m_isSaving = true;
		MultiThread::Async(L"save download content", [&]() {
			ZIPReader zipReader(std::move(m_byte));
			zipReader.extractAll(m_pContent->getSaveLocalPath());
			m_pContent->setDownloaded(true);
			bool isUpdate = DownloadHistory::Add(m_pContent->getDownloadId());
			if (isUpdate) {
				DownloadHistory::Save();
			}
			m_pContent = nullptr;
			m_byte = ByteArray();
			m_isSaving = false;
			m_isActive = false;
		});
		return true;
	}
	bool download(DownloadContent& content)
	{
		if (!Internet::IsConnected()) {
			return false;
		}
		m_pContent = &content;
		m_isActive = true;
		return m_client.downloadAsync(Setting::DOWNLOAD_URL + m_pContent->getDownloadUrl(), m_byte);
	}
	bool downloadUpdate()
	{
		if (m_isSaving) {
			return true;
		}
		const auto progress = m_client.retreiveProgress();
		if (progress.status == DownloadStatus::Working) {
			return true;
		}
		else if (progress.status == DownloadStatus::Succeeded) {
			this->save();
		}
		else if (m_byte) {
			// すでに完了
			this->save();
		}
		return false;
	}
	bool isActive()
	{
		return m_isActive;
	}
	double getProgress()
	{
		return m_client.retreiveProgress().getProgress().value_or(m_isActive?1.0 : 0.0);
	}
};
Downloader::Downloader():
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
