﻿#pragma once
#include <scenes/Scene/FileLoad/FileLoadScene.hpp>
#include <Useful.hpp>
#include <utils/Thread/Task.hpp>
#include <utils/Coro/Fiber/FiberUtil.hpp>
#include <utils/Asset/SivAssetUtil.hpp>

#include <core/Data/MusicData/MusicData.hpp>
#include <core/Data/Genre/GenreManager.hpp>
#include <core/Data/Genre/CostumFolder.hpp>
#include <core/Data/CourseData/CourseData.hpp>
#include <core/Data/CourseGenre/CourseGenre.hpp>
#include <core/Data/TapSE/TapSE.hpp>
//#include "CompatibilityUtil.hpp"
#include <Siv3D.hpp>


namespace
{
	using namespace ct;
	static double g_loadingRate = 0;

    //--------------------------------------------------------------------------------
    //関数：LoadMusicDatas
    //--------------------------------------------------------------------------------
    //概要:楽曲のロード
    //--------------------------------------------------------------------------------
	void LoadCourses(const std::stop_token& stopSource);
	//se読み込み
	void LoadTapSE(const std::stop_token& stopSource)
	{
		auto& tapSEs = Game::TapSEs();
		tapSEs.clear();
		static const FilePath& nonePath = U"Resource/Sound/SE/none.mp3";
		tapSEs.emplace_back(U"なし", nonePath, nonePath, nonePath);

		for (auto&& rootFilePath : FileSystem::DirectoryContents(U"TapSE")) {
			if (stopSource.stop_requested()) {
				return;
			}
			if (FileSystem::IsDirectory(rootFilePath)) {
				tapSEs.emplace_back(rootFilePath);
			}
		}
	}
	void LoadMusicDatas(const std::stop_token& stopSource)
	{
		// TODO
		// スコアを移動
		//CompatibilityUtil::MoveScoreFolder();

		g_loadingRate = 0;
		Array<MusicData>& musics = Game::Musics();
		SivAssetUtil::UnregisterByTag<AudioAsset>(U"MusicData");
		musics.clear();

		GenreManager::Clear();
		const auto genrePaths = FileSystem::DirectoryContents(U"Music");
		MusicData::Index = 0;

		size_t musicSize = 0;
		for (const auto& gPath : genrePaths) {

			const auto musicPaths = FileSystem::DirectoryContents(gPath);
			musicSize += musicPaths.size();
		}

		musics.reserve(musicSize);

		size_t curIndex = 0;
		//ここから楽曲データ読み込み
		for (const auto& gPath : genrePaths) {

			const auto musicPaths = FileSystem::DirectoryContents(gPath);
			const auto genreName = FileSystem::BaseName(gPath);
			for (const auto& path : musicPaths) {
				//各楽曲に入っているアセットのパス取得
				auto assets = FileSystem::DirectoryContents(path);
				//iniファイルがあるか検索
				for (const auto& elm : assets) {
					if (stopSource.stop_requested()) {
						return;
					}
					if (FileSystem::Extension(elm) == U"ini") {
						g_loadingRate = curIndex / static_cast<double>(musicSize);
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
		::LoadCustomFolder(stopSource);

		GenreManager::Add(GenreType::All, U"ALL", []([[maybe_unused]]MusicData& music)->bool {return true; });
		GenreManager::Add(GenreType::Favorite, U"お気に入り", [](MusicData& music)->bool {return  music.isFavorite(); });
		GenreManager::Sort();

		//タップSE読み込み
		::LoadTapSE(stopSource);

		::LoadCourses(stopSource);
		ClearPrint();
	}
	//コースデータ読み込み
	void LoadCourses(const std::stop_token& stopSource)
	{
		CourseGenreManager::Clear();
		CourseData::Index = 0;

		Array<CourseData>& courses = Game::Courses();
		courses.clear();

		const auto genreFiles = FileSystem::DirectoryContents(U"Course");

		size_t coureSize = 0;

		//リサーブ
		for (const auto& gPath : genreFiles) {
			const auto ctcPaths = FileSystem::DirectoryContents(gPath);
			for (const auto& path : ctcPaths) {
				if (FileSystem::Extension(path) == U"ctc") {
					++coureSize;
				}
			}
		}
		courses.reserve(coureSize);

		//ここから楽曲データ読み込み
		for (const auto& gPath : genreFiles) {
			const auto genreName = FileSystem::BaseName(gPath);

			const auto ctcPaths = FileSystem::DirectoryContents(gPath);
			for (const auto& path : ctcPaths) {
				if (stopSource.stop_requested()) {
					return;
				}
				if (FileSystem::Extension(path) == U"ctc") {
					courses.emplace_back(path);
				}
			}
			CourseGenreManager::Add(genreName);
		}
	}
}
namespace ct
{
    //--------------------------------------------------------------------------------
    //関数：コンストラクタ
    //--------------------------------------------------------------------------------
	FileLoadScene::FileLoadScene(const InitData& init) :
		ISceneBase(init),
		m_view(this)
	{
		getData().m_scrollRate = Game::Config().m_scrollRate;

		m_asyncUpdater.reset(std::bind(&FileLoadScene::updateAsync, this));
	}

	//--------------------------------------------------------------------------------
	//関数：デストラクタ
	//--------------------------------------------------------------------------------
	FileLoadScene::~FileLoadScene()
	{
		ClearPrint();
	}
	//--------------------------------------------------------------------------------
	//関数：update
	//--------------------------------------------------------------------------------
	void FileLoadScene::update()
	{
		if (m_state == State::Loading) {
			m_view.update();
		}
		m_asyncUpdater.resume();
	}

	//--------------------------------------------------------------------------------
	//関数：draw
	//--------------------------------------------------------------------------------
	void FileLoadScene::draw()const
	{
		// ローディングの描画
		m_view.draw();
	}

	void FileLoadScene::drawFadeIn(double t) const
	{
		if (getData().m_fromScene == SceneName::Title) {
			FadeOut(Fade::FlipPage, t, [this]() {draw(); }, false);
		} else {
			FadeIn(Fade::FlipPage, t, [this]() {draw(); }, true);
		}
	}

	//--------------------------------------------------------------------------------
	//関数：drawFadeOut
	//--------------------------------------------------------------------------------
	void FileLoadScene::drawFadeOut([[maybe_unused]]double t) const
	{
		draw();
	}

	double FileLoadScene::progress() const
	{
		return g_loadingRate;
	}

	Coro::Fiber<void> FileLoadScene::updateAsync()
	{
		// ロード開始
		co_await Thread::Task(::LoadMusicDatas);

		// ロード完了
		m_state = State::LoadCompleted;
		m_view.onCompleted();

		co_await Coro::FiberUtil::WaitForSeconds(0.8s);

		// シーン遷移
		auto& musics = Game::Musics();
		if (musics.size() == 0) {
			// 楽曲なし
			System::Exit();
		} else {
			changeScene(SceneName::Title, 1000);
		}
	}
}
