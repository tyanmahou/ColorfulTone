#pragma once
#include<Siv3D/String.hpp>

class DownloadContent
{
private:
	s3d::uint32 m_downloadId; // id
	s3d::URL m_downloadURL; // download����url
	s3d::FilePath m_saveLocalPath; // �ۑ�����p�X
	bool m_isDownloaded = false; // ���łɃ_�E�����[�h�ς݂�
public:
};