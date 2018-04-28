#pragma once

#include<stack>

#include"FileLoad.h"
#include"Fade.h"
#include"GenreManager.h"
#include"VideoAsset.h"

namespace
{
	std::mutex g_mutex;

	static float g_loadingRate =0;
}
std::mutex& GetMutex()
{
	return g_mutex;
}
//--------------------------------------------------------------------------------
//関数：LoadMusicDatas
//--------------------------------------------------------------------------------
//概要:楽曲のロード
//--------------------------------------------------------------------------------
void LoadCourses();
void LoadCustomFolder();

//se読み込み
void LoadTapSE()
{
	g_mutex.lock();
	for (auto&&rootFilePath : FileSystem::DirectoryContents(L"TapSE"))
	{
		if (FileSystem::IsDirectory(rootFilePath))
		{
			for (auto&& filePath : FileSystem::DirectoryContents(rootFilePath))
			{
				Game::Instance()->m_tapSEPaths.emplace_back(filePath);
			}
		}
		Game::Instance()->m_tapSEPaths.emplace_back(rootFilePath);
	}

	g_mutex.unlock();

	Erase_if(Game::Instance()->m_tapSEPaths, [](const String& path) {return Audio::GetFormat(path) == AudioFormat::Unknown; });


}
void LoadMusicDatas()
{
	g_loadingRate = 0;
	Array<MusicData>& musics = Game::Instance()->m_musics;

	SoundAsset::UnregisterByTag(L"MusicData");
	musics.clear();
	GenreManager::m_refiners.clear();
	const auto genrePaths = FileSystem::DirectoryContents(L"Music");
	MusicData::Index = 0;

	unsigned int musicSize = 0;
	for (const auto& gPath : genrePaths)
	{

		const auto musicPaths = FileSystem::DirectoryContents(gPath);
		musicSize += musicPaths.size();
	}

	musics.reserve(musicSize);

	int curIndex = 0;
	//ここから楽曲データ読み込み
	for (const auto& gPath : genrePaths)
	{

		const auto musicPaths = FileSystem::DirectoryContents(gPath);
		const auto genreName = FileSystem::BaseName(gPath);
		for (const auto& path : musicPaths)
		{
			//各楽曲に入っているアセットのパス取得
			auto assets = FileSystem::DirectoryContents(path);
			//iniファイルがあるか検索
			for (const auto& elm : assets)
			{
				if (System::GetPreviousEvent() == ((Input::KeyAlt + Input::KeyF4).clicked | WindowEvent::CloseButton))
					return;
				if (elm.includes(L"ini"))
				{

					g_mutex.lock();
					g_loadingRate = curIndex / static_cast<float>(musicSize);
					//Println(path);
					musics.emplace_back(genreName, path, elm);
					g_mutex.unlock();

					GenreManager::Add(GenreType::Folder, genreName, [genreName](MusicData& music)->bool {return !(music.getGenreName() == genreName); });
					break;
				}
			}
			++curIndex;
		}
	}

	//カスタムフォルダ読み込み
	::LoadCustomFolder();

	GenreManager::Add(GenreType::All, L"ALL", [](MusicData& music)->bool {return false; });
	GenreManager::Sort();

	//タップSE読み込み
	::LoadTapSE();

	g_mutex.lock();
	::LoadCourses();
	ClearPrint();
	Println(L"Complete");
	g_mutex.unlock();
	Game::Instance()->m_isMusicLoadEnd = true;
}

//比較演算の決定をして条件式とする
template<class _T>
std::function<bool(MusicData& music)> OperationSet(const String& op, const _T& (MusicData::*f)()const, const _T& value)
{
	std::function<bool(MusicData& music)> func = [](MusicData& m)->bool {return true; };

	func = [value, f, op](MusicData& music)->bool {return CompareForOperator((music.*f)(), value, op); };

	return func;
}

//論理演算の結合
void ChainSet(const String& chain, std::function<bool(MusicData& music)>&func, std::function<bool(MusicData& music)>&f)
{
	if (chain == L"OR")
	{
		func = [func, f](MusicData& music)->bool {return func(music) || f(music); };
	}
	else
	{
		func = [func, f](MusicData& music)->bool {return func(music) && f(music); };
	}

}

template<class _T>
void FunctionSet(CSVReader&csv,int index, const _T& (MusicData::*memfunc)()const, std::function<bool(MusicData& music)>&func,String& chain)
{
	const _T value = csv.get<_T>(index, 2);
	const String op = csv.get<String>(index, 1);

	auto f = ::OperationSet(op, memfunc, value);

	::ChainSet(chain, func, f);

	chain = L"AND";

}

template<class _T>
void FunctionSetForNotesData(CSVReader&csv, int index, bool (MusicData::*memfunc)(const _T&,const String&)const, std::function<bool(MusicData&)>&func, String& chain)
{
	const _T value = csv.get<_T>(index, 2);
	const String op = csv.get<String>(index, 1);

	std::function<bool(MusicData&)>f = [value, memfunc, op](MusicData& music)->bool {return (music.*memfunc)(value,op); };

	::ChainSet(chain, func, f);

	chain = L"AND";

}

void LoadCustomFolder()
{
	const auto ctcfFiles = FileSystem::DirectoryContents(L"CustomFolder");

	//ここからデータ読み込み
	for (const auto& path : ctcfFiles)
	{
		if (path.includes(L"ctfolder"))
		{

			CSVReader csv(path);

			if (!csv)
				continue;

			//タイトル
			String title = L"CustomFolder";

			std::function<bool(MusicData& music)> func = [](MusicData& music)->bool {return true; };


			auto rows = csv.rows;				//行数
			String head;						//1列目のデータを文字列で
			String chain = L"AND";				//AND or OR
			std::stack<std::pair<std::function<bool(MusicData& music)>, String>> stack;//()用

			for (unsigned int i = 0; i < rows; ++i)
			{

				head = csv.get<String>(i, 0);

				if (head.isEmpty)			//空行はスルー
				{
					continue;
				}

				if (head[0] == '%')//コメント
				{
					continue;
				}
				if (head==L"(")
				{
					stack.emplace(func, chain);
					func= [](MusicData& music)->bool {return true; };
					chain = L"AND";
				}
				if (head == L")")
				{
					auto& top = stack.top();
					ChainSet(top.second, func,top.first);
					chain = L"AND";
					stack.pop();
				}
				if (head == L"#TITLE")
				{
					title = csv.get<String>(i, 1);
				}

				if (head == L"AND" || head == L"OR")
				{
					chain = head;
				}
				if (head == L"BPM" || head == L"MINBPM")
				{
					::FunctionSet(csv, i, &(MusicData::getMinBPM), func, chain);
				}

				if (head == L"MAXBPM")
				{
					::FunctionSet(csv, i, &(MusicData::getMaxBPM), func, chain);
				}

				if (head == L"ARTIST")
				{
					::FunctionSet(csv, i, &(MusicData::getArtistName), func, chain);
				}
				if (head == L"MUSICNAME")
				{
					::FunctionSet(csv, i, &(MusicData::getMusicName), func, chain);
				}
				if (head == L"GENRE")
				{
					::FunctionSet(csv, i, &(MusicData::getGenreName), func, chain);
				}
				if (head == L"LEVEL")
				{
					::FunctionSetForNotesData(csv, i, &(MusicData::serchLevel), func, chain);
				}
				if (head == L"LEVELNAME")
				{
					::FunctionSetForNotesData(csv, i, &(MusicData::serchLevelName), func, chain);
				}
				if (head == L"NOTESARTIST")
				{
					::FunctionSetForNotesData(csv, i, &(MusicData::serchNotesArtistName), func, chain);
				}
				if (head == L"TOTALNOTE")
				{
					::FunctionSetForNotesData(csv, i, &(MusicData::serchTotalNotes), func, chain);
				}
				if (head == L"CLEARRATE")
				{
					::FunctionSetForNotesData(csv, i, &(MusicData::serchClearRate), func, chain);
				}
				if (head == L"CLEAR")
				{
					::FunctionSetForNotesData(csv, i, &(MusicData::serchClear), func, chain);
				}
				if (head == L"SPECIAL")
				{
					const String value = csv.get<String>(i, 2);
					const String op = csv.get<String>(i, 1);

					SpecialResult s;
					if (value == L"AP")
						s = SpecialResult::All_Perfect;
					if (value == L"FC")
						s = SpecialResult::Full_Combo;


					std::function<bool(MusicData& music)>f = [value, op,s](MusicData& music)->bool {return music.serchSpecialResult(s, op); };

					::ChainSet(chain, func, f);

					chain = L"AND";

				}



			}



			GenreManager::Add(GenreType::Custom, title, [func](MusicData& music)->bool {return !func(music); });


		}
	}

}

//コースデータ読み込み
void LoadCourses()
{
	Array<CourseData>& courses = Game::Instance()->m_courses;

	courses.clear();

	const auto genreFiles = FileSystem::DirectoryContents(L"Course");

	unsigned int musicSize = 0;

	//リサーブ
	//courses.reserve(ctcFiles.size());

	//ここから楽曲データ読み込み
	for (const auto& gPath : genreFiles)
	{
		const auto ctcPaths = FileSystem::DirectoryContents(gPath);
		for (const auto& path : ctcPaths)
		{
			if (path.includes(L"ctc"))
			{
				courses.emplace_back(path);
			}
		}
	}

}

//--------------------------------------------------------------------------------
//静的メンバ定義
//--------------------------------------------------------------------------------
std::future<void> FileLoad::m_loadResult = std::async(std::launch::async, []() {return; });

//--------------------------------------------------------------------------------
//関数：コンストラクタ
//--------------------------------------------------------------------------------
FileLoad::FileLoad() :m_timer(0), m_font(15)
{
	m_loadResult = std::async(std::launch::async, LoadMusicDatas);

}

//--------------------------------------------------------------------------------
//関数：デストラクタ
//--------------------------------------------------------------------------------
FileLoad::~FileLoad()
{
	ClearPrint();
}

void FileLoad::init()
{

	m_data->m_scrollRate = Game::Instance()->m_config.m_scrollRate;

}
//--------------------------------------------------------------------------------
//関数：update
//--------------------------------------------------------------------------------
void FileLoad::update()
{
	m_timer++;

	if (Game::Instance()->m_isMusicLoadEnd)
	{
		auto &musics = Game::Instance()->m_musics;
		if (musics.size() == 0)
			System::Exit();

		changeScene(L"title", 3000);

	}
}

//--------------------------------------------------------------------------------
//関数：draw
//--------------------------------------------------------------------------------
void FileLoad::draw()const
{
	static auto& v = Mahou::VideoAsset(L"loading");
	if (!v.isPlaying())
		v.play();

	v.update();

	v.getFrameTexture().draw();

	Rect(0,570, 800 * g_loadingRate, 30).draw(Color(0, 0, 0, 128));

}

void FileLoad::drawFadeIn(double t) const
{
	draw();
	FadeIn(t, Fade::Default);
}




//--------------------------------------------------------------------------------
//関数：drawFadeOut
//--------------------------------------------------------------------------------
void FileLoad::drawFadeOut(double t) const
{
	draw();
	FadeOut(t, Fade::SmoothCircle);
}
