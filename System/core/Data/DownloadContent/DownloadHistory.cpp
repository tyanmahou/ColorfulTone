#include <core/Data/DownloadContent/DownloadHistory.hpp>
#include <Siv3D.hpp>
namespace
{
	constexpr auto g_path = U"UserData/System/download_history.cts";
}

namespace ct
{
	class DownloadHistory::Impl
	{
	private:
		s3d::HashSet<s3d::uint32> m_history;
	public:
		Impl()
		{
			this->load();
		}
		bool load()
		{
			TextReader tex(g_path);
			if (!tex) {
				return false;
			}
			String out;
			while (tex.readLine(out)) {
				if (auto id = ParseOpt<uint32>(out); id) {
					m_history.emplace(*id);
				}
			}
			return true;
		}
		bool hasContain(uint32 downloadId)const
		{
			return m_history.find(downloadId) != m_history.end();
		}
		bool save()const
		{
			TextWriter writer(g_path);
			for (auto&& elm : m_history) {
				writer.writeln(elm);
			}
			return true;
		}
		bool add(uint32 downloadId)
		{
			auto result = m_history.emplace(downloadId);
			return result.second;
		}
	};
	DownloadHistory::DownloadHistory() :
		m_pImpl(std::make_shared<Impl>())
	{}
	bool DownloadHistory::Load()
	{
		return Instance()->m_pImpl->load();
	}

	bool DownloadHistory::HasContein(uint32 downloadId)
	{
		return Instance()->m_pImpl->hasContain(downloadId);
	}

	bool DownloadHistory::Add(uint32 downloadId)
	{
		return Instance()->m_pImpl->add(downloadId);
	}

	bool DownloadHistory::Save()
	{
		return Instance()->m_pImpl->save();
	}
}
