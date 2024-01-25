#pragma once
#include <memory>
#include <utils/Singleton/DynamicSingleton.hpp>
#include <Siv3D/Fwd.hpp>

namespace ct
{
	class DownloadHistory : protected DynamicSingleton<DownloadHistory>
	{
		friend class DynamicSingleton<DownloadHistory>;
	public:
		static bool Load();
		static bool HasContein(s3d::uint32 downloadId);
		static bool Add(s3d::uint32 downloadId);
		static bool Save();
	private:
		class Impl;
		std::shared_ptr<Impl> m_pImpl;
		DownloadHistory();
	};
}