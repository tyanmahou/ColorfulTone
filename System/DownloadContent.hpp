#pragma once
#include<Siv3D/String.hpp>

class DownloadContent
{
private:
	s3d::uint32 m_downloadId; // id
	s3d::URL m_downloadURL; // downloadするurl
	s3d::FilePath m_saveLocalPath; // 保存するパス
	bool m_isDownloaded = false; // すでにダウンロード済みか
public:
};