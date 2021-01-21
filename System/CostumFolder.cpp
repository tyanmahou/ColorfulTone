#include"CostumFolder.hpp"
#include"CTCFReader.hpp"
#include"MusicData.h"
#include"GenreManager.h"

namespace
{
	std::unordered_map<String, CTCFReader> g_readerCache;
}
void LoadCustomFolder()
{
	g_readerCache.clear();

	const auto ctcfFiles = FileSystem::DirectoryContents(L"CustomFolder");

	//ここからデータ読み込み
	for (const auto& path : ctcfFiles)
	{
		if (FileSystem::Extension(path) == L"ctfolder")
		{
			CTCFReader ctfolder(path);
			if (!ctfolder)
			{
				continue;
			}
			//タイトル
			String title = ctfolder.getTitle().value_or(L"CustomFolder");
			g_readerCache[path] = ctfolder;
			GenreManager::Add(GenreType::Custom, title, [path](MusicData& music)->bool {return g_readerCache[path].expression(music); });
		}
	}
}
