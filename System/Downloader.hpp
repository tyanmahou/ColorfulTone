#pragma once
#include<memory>

class DownloadContent;

class Downloader
{
private:
	class Impl;
	std::shared_ptr<Impl> m_pImpl;
public:
	Downloader();
	bool isActive()const;
	// �_�E�����[�h�i��
	bool downloadUpdate()const;
	// �_�E�����[�h�J�n
	bool download(DownloadContent& content)const;
	// �_�E�����[�h��
	double getProgress() const;
};