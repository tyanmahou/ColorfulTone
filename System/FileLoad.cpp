#pragma once
#include"FileLoad.h"
#include "Useful.hpp"

#include"GenreManager.h"
#include"CostumFolder.hpp"
#include"CourseGenre.hpp"
#include"MultiThread.hpp"
namespace
{
	static double g_loadingRate = 0;
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
	for (auto&&rootFilePath : FileSystem::DirectoryContents(L"TapSE"))
	{
		if (FileSystem::IsDirectory(rootFilePath))
		{
			for (auto&& filePath : FileSystem::DirectoryContents(rootFilePath))
			{
				Game::TapSEPaths().emplace_back(filePath);
			}
		}
		Game::TapSEPaths().emplace_back(rootFilePath);
	}
	Erase_if(Game::TapSEPaths(), [](const String& path) {return Audio::GetFormat(path) == AudioFormat::Unknown; });
}
void LoadMusicDatas()
{
	g_loadingRate = 0;
	Array<MusicData>& musics = Game::Musics();

	SoundAsset::UnregisterByTag(L"MusicData");
	musics.clear();
	GenreManager::Clear();
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
					g_loadingRate = curIndex / static_cast<float>(musicSize);
					//Println(path);
					musics.emplace_back(genreName, path, elm);
					GenreManager::Add(GenreType::Folder, genreName, [genreName](MusicData& music)->bool {return music.getGenreName() == genreName; });
					break;
				}
			}
			++curIndex;
		}
	}

	//カスタムフォルダ読み込み
	::LoadCustomFolder();

	GenreManager::Add(GenreType::All, L"ALL", [](MusicData& music)->bool {return true; });
	GenreManager::Sort();

	//タップSE読み込み
	::LoadTapSE();

	::LoadCourses();
	ClearPrint();
	Game::SetLoadCompleted(true);
}
//コースデータ読み込み
void LoadCourses()
{
	CourseGenreManager::Clear();
	CourseData::Index = 0;

	Array<CourseData>& courses = Game::Courses();

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
FileLoad::FileLoad():
	m_timer(0),
	m_state(State::Loading)
{
	Game::SetLoadCompleted(false);
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
	m_data->m_scrollRate = Game::Config().m_scrollRate;
}
//--------------------------------------------------------------------------------
//関数：update
//--------------------------------------------------------------------------------
void FileLoad::update()
{
	m_timer++;
	if (m_state == State::Loading)
	{
		m_view.update();
		if (Game::IsLoadCompleted())
		{
			m_view.onCompleted();
			m_state = State::OnLoadCompleted;
		}
	}
	else if (m_state == State::OnLoadCompleted)
	{
		if (m_view.getStopwatchMs() >= 800)
		{
			auto &musics = Game::Musics();
			if (musics.size() == 0)
			{
				System::Exit();
			}
			changeScene(L"title", 1000);
		}
	}

}

//--------------------------------------------------------------------------------
//関数：draw
//--------------------------------------------------------------------------------
void FileLoad::draw()const
{
	// ローディングの描画
	m_view.draw();
}

void FileLoad::drawFadeIn(double t) const
{
	if (m_data->m_fromScene == SceneName::Title)
	{
		FadeOut(Fade::FlipPage, t, [this]() {draw(); }, false);
	}
	else
	{
		FadeIn(Fade::FlipPage, t, [this]() {draw(); }, true);
	}
}

//--------------------------------------------------------------------------------
//関数：drawFadeOut
//--------------------------------------------------------------------------------
void FileLoad::drawFadeOut(double t) const
{
	draw();
}

double FileLoad::GetProgress()
{
	return g_loadingRate;
}
