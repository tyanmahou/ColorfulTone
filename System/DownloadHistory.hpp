#pragma once
#include<memory>

#include"Singleton.h"

#include<Siv3D/Fwd.hpp>

class DownloadHistory:protected Singleton<DownloadHistory>
{
	class Impl;
	std::shared_ptr<Impl> m_pImpl;
	DownloadHistory();
public:
	static bool Load();
	static bool HasContein(s3d::int32 downloadId);
	static bool Add(s3d::int32 downloadId);
	static bool Save();
	friend class Singleton<DownloadHistory>;
};