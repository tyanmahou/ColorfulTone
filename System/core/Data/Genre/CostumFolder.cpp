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
	s3d::Array<GenreData> LoadCustomFolder(const std::stop_token& stopToken)
	{
		s3d::Array<GenreData> results;
		g_readerCache.clear();

		const auto ctcfFiles = FileSystem::DirectoryContents(U"CustomFolder", Recursive::No);

		//ここからデータ読み込み
		for (const auto& path : ctcfFiles) {
			if (stopToken.stop_requested()) {
				return results;
			}
			if (FileSystem::Extension(path) == U"ctfolder") {
				CTCFReader ctfolder(path);
				if (!ctfolder) {
					continue;
				}
				//タイトル
				g_readerCache[path] = ctfolder;
				results.push_back(GenreData::CreateCustom(ctfolder));
			}
		}
		return results;
	}
}
