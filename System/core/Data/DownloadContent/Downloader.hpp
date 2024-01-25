#pragma once
#include <memory>

namespace ct
{
	class DownloadContent;

	class Downloader
	{
	private:
		class Impl;
		std::shared_ptr<Impl> m_pImpl;
	public:
		Downloader();
		bool isActive()const;
		// ダウンロード進捗
		bool downloadUpdate()const;
		// ダウンロード開始
		bool download(DownloadContent& content)const;
		// ダウンロード率
		double getProgress() const;
	};
}
