#include"CostumFolder.hpp"
#include"MusicData.h"
#include"GenreManager.h"
#include<stack>

void LoadCustomFolder()
{
	const auto ctcfFiles = FileSystem::DirectoryContents(L"CustomFolder");

	//ここからデータ読み込み
	//for (const auto& path : ctcfFiles)
	//{
	//	if (path.includes(L"ctfolder"))
	//	{

	//		CSVReader csv(path);

	//		if (!csv)
	//			continue;

	//		//タイトル
	//		String title = L"CustomFolder";

	//		GenreManager::Add(GenreType::Custom, title, [func](MusicData& music)->bool {return !func(music); });
	//	}
	//}
}
