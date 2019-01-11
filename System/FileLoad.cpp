#pragma once

#include"FileLoad.h"
#include"Fade.h"
#include"GenreManager.h"
#include"VideoAsset.h"
#include"CostumFolder.hpp"
#include"CourseGenre.hpp"
#include"MultiThread.hpp"
namespace
{
	static float g_loadingRate = 0;
}
//--------------------------------------------------------------------------------
//関数：LoadMusicDatas
//--------------------------------------------------------------------------------
//概要:楽曲のロード
//--------------------------------------------------------------------------------
void LoadCourses();
//se読み込み
void LoadTapSE()
{
	auto&& mutex = MultiThread::GetMutex();
	mutex.lock();
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

	mutex.unlock();

	Erase_if(Game::Instance()->m_tapSEPaths, [](const String& path) {return Audio::GetFormat(path) == AudioFormat::Unknown; });
}
void LoadMusicDatas()
{
	auto&& mutex = MultiThread::GetMutex();

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

					mutex.lock();
					g_loadingRate = curIndex / static_cast<float>(musicSize);
					//Println(path);
					musics.emplace_back(genreName, path, elm);
					mutex.unlock();

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

	mutex.lock();
	::LoadCourses();
	ClearPrint();
	mutex.unlock();
	Game::Instance()->m_isMusicLoadEnd = true;
}



//コースデータ読み込み
void LoadCourses()
{
	CourseGenreManager::Clear();
	CourseData::Index = 0;

	Array<CourseData>& courses = Game::Instance()->m_courses;

	courses.clear();

	const auto genreFiles = FileSystem::DirectoryContents(L"Course");

	unsigned int musicSize = 0;

	//リサーブ
	//courses.reserve(ctcFiles.size());

	//ここから楽曲データ読み込み
	for (const auto& gPath : genreFiles)
	{
		const auto genreName = FileSystem::BaseName(gPath);

		const auto ctcPaths = FileSystem::DirectoryContents(gPath);
		for (const auto& path : ctcPaths)
		{
			if (path.includes(L"ctc"))
			{
				courses.emplace_back(path);
			}
		}
		CourseGenreManager::Add(genreName);
	}
}

//--------------------------------------------------------------------------------
//静的メンバ定義
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//関数：コンストラクタ
//--------------------------------------------------------------------------------
FileLoad::FileLoad() :
	m_timer(0)
{
	MultiThread::Async(L"file_load", LoadMusicDatas);
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
	m_view.update();
	if (Game::Instance()->m_isMusicLoadEnd)
	{
		m_view.onCompleted();
		auto &musics = Game::Instance()->m_musics;
		if (musics.size() == 0)
			System::Exit();

		changeScene(L"title", 4000);
	}
}

//--------------------------------------------------------------------------------
//関数：draw
//--------------------------------------------------------------------------------
void FileLoad::draw()const
{
	// ローディングの描画
	m_view.draw();

	Rect(0, 570, 800 * g_loadingRate, 30).draw(Color(0, 0, 0, 128));
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
	//FadeOut(t, Fade::SmoothCircle);
}
