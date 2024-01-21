#include <core/Data/Genre/CostumFolder.hpp>
#include <core/Data/Genre/CTCFReader.hpp>
#include <core/Data/MusicData/MusicData.hpp>
#include <core/Data/Genre/GenreManager.hpp>
#include <Siv3D.hpp>

namespace
{
	std::unordered_map<String, ct::CTCFReader> g_readerCache;
}
namespace ct
{
	void LoadCustomFolder(const std::stop_token& stopSource)
	{
		g_readerCache.clear();

		const auto ctcfFiles = FileSystem::DirectoryContents(U"CustomFolder");

		//ここからデータ読み込み
		for (const auto& path : ctcfFiles) {
			if (stopSource.stop_requested()) {
				return;
			}
			if (FileSystem::Extension(path) == U"ctfolder") {
				CTCFReader ctfolder(path);
				if (!ctfolder) {
					continue;
				}
				//タイトル
				String title = ctfolder.getTitle().value_or(U"CustomFolder");
				int32 order = ctfolder.getOrder();
				g_readerCache[path] = ctfolder;
				GenreManager::Add(GenreType::Custom, title, [path](const MusicData& music)->bool {return g_readerCache[path].expression(music); }, order);
			}
		}
	}
}
