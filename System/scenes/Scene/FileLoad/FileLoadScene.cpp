#pragma once
#include <scenes/Scene/FileLoad/FileLoadScene.hpp>
#include <Useful.hpp>
#include <utils/Thread/Task.hpp>
#include <utils/Coro/Fiber/FiberUtil.hpp>
#include <utils/Asset/SivAssetUtil.hpp>

#include <core/Data/Genre/CostumFolder.hpp>
#include <core/Data/TapSE/TapSE.hpp>
#include <commons/Migration/MigrationSystem.hpp>
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
	bool LoadMusicData(const std::stop_token& stopToken)
	{
		g_loadingRate = 0;
		// ジャンル予約用
		s3d::HashSet<int32> lvSet;
		s3d::HashSet<StarLv> starLvSet;
		s3d::HashSet<std::pair<String, bool>> folderSet;

		Array<MusicData>& musics = Game::Musics();
		SivAssetUtil::UnregisterByTag<AudioAsset>(U"MusicData");
		musics.clear();

		GenreManager::Clear();
		const auto genrePaths = FileSystem::DirectoryContents(U"Music", Recursive::No);
		MusicData::ResetIndex();

		size_t musicSize = 0;
		for (const auto& gPath : genrePaths) {

			const auto musicPaths = FileSystem::DirectoryContents(gPath, Recursive::No);
			musicSize += musicPaths.size();
		}

		musics.reserve(musicSize);

		size_t curIndex = 0;
		//ここから楽曲データ読み込み
		for (const auto& gPath : genrePaths) {
			bool foundMusic = false;
			const auto musicPaths = FileSystem::DirectoryContents(gPath, Recursive::No);
			const auto genreName = FileSystem::BaseName(gPath);

			const bool isOfficial = !U"^\\d{4}_\\d{2}"_re.match(genreName).isEmpty();
			for (const auto& path : musicPaths) {
				//各楽曲に入っているアセットのパス取得
				auto assets = FileSystem::DirectoryContents(path, Recursive::No);
				//iniファイルがあるか検索
				for (const auto& elm : assets) {
					if (stopToken.stop_requested()) {
						return false;
					}
					if (FileSystem::Extension(elm) == U"ini") {
						//Println(path);
						MusicData& music = musics.emplace_back(genreName, path, elm, isOfficial);

						for (const NotesData& notes : music.getNotesData()) {
							if (!notes.isValid()) {
								continue;
							}
							StarLv starLv = notes.getStarLv();
							if (starLv != StarLv::None) {
								starLvSet.insert(starLv);
							} else {
								int32 lv = notes.getLevel();
								lvSet.insert(lv);
							}
						}

						g_loadingRate = curIndex / static_cast<double>(musicSize);
						foundMusic = true;
						break;
					}
				}
				++curIndex;
			}

			if (foundMusic) {
				// ジャンル登録
				folderSet.emplace(genreName, isOfficial);
			}
		}

		// ジャンル
		{
			Array<GenreData> genreRserves;
			for (int32 lv : lvSet) {
				genreRserves << GenreData::CreateLv(lv);
			}
			for (StarLv starLv : starLvSet) {
				genreRserves << GenreData::CreateStarLv(starLv);
			}
			for (const auto&[ folder, isOfficial] : folderSet) {
				genreRserves << GenreData::CreateFolder(folder, isOfficial);
			}
			//カスタムフォルダ読み込み
			genreRserves.append(::LoadCustomFolder(stopToken));

			genreRserves << GenreData::CreateAll();
			genreRserves << GenreData::CreateRandom();
			genreRserves << GenreData::CreateFavorite();

			GenreManager::Reflesh(genreRserves);
		}
		return true;
	}
	//コースデータ読み込み
	bool LoadCourses(const std::stop_token& stopToken)
	{
		//g_loadingRate = 0;
		CourseGenreManager::Clear();
		CourseData::Index = 0;

		Array<CourseData>& courses = Game::Courses();
		courses.clear();

		const auto genreFiles = FileSystem::DirectoryContents(U"Course", Recursive::No);

		size_t coureSize = 0;

		//リサーブ
		for (const auto& gPath : genreFiles) {
			const auto ctcPaths = FileSystem::DirectoryContents(gPath, Recursive::No);
			for (const auto& path : ctcPaths) {
				if (FileSystem::Extension(path) == U"ctc") {
					++coureSize;
				}
			}
		}
		courses.reserve(coureSize);

		//ここから楽曲データ読み込み
		size_t curIndex = 0;
		for (const auto& gPath : genreFiles) {
			const auto genreName = FileSystem::BaseName(gPath);

			const auto ctcPaths = FileSystem::DirectoryContents(gPath, Recursive::No);
			for (const auto& path : ctcPaths) {
				if (stopToken.stop_requested()) {
					return false;
				}
				if (FileSystem::Extension(path) == U"ctc") {
					courses.emplace_back(path);
					//g_loadingRate = curIndex / static_cast<double>(coureSize);
					++curIndex;
				}
			}
			CourseGenreManager::Add(genreName);
		}
		return true;
	}

	//se読み込み
	bool LoadTapSE(const std::stop_token& stopToken)
	{
		//g_loadingRate = 0;
		auto& tapSEs = Game::TapSEs();
		tapSEs.clear();

		size_t size = 1; // noneの分+1
		for (auto&& rootFilePath : FileSystem::DirectoryContents(U"TapSE", Recursive::No)) {
			if (FileSystem::IsDirectory(rootFilePath)) {
				size += 1;
			}
		}
		tapSEs.reserve(size);

		const FilePath nonePath = U"Resource/Sound/SE/none.mp3";
		tapSEs.emplace_back(U"なし", nonePath, nonePath, nonePath);

		size_t curIndex = 1;
		//g_loadingRate = curIndex / static_cast<double>(size);

		for (auto&& rootFilePath : FileSystem::DirectoryContents(U"TapSE", Recursive::No)) {
			if (stopToken.stop_requested()) {
				return false;
			}
			if (FileSystem::IsDirectory(rootFilePath)) {
				tapSEs.emplace_back(rootFilePath);
				//g_loadingRate = curIndex / static_cast<double>(size);
				++curIndex;
			}
		}
		return true;
	}

	void LoadContentsData(const std::stop_token& stopToken)
	{
		if (!LoadMusicData(stopToken)) {
			return;
		}
		if (!LoadCourses(stopToken)) {
			return;
		}
		if (!LoadTapSE(stopToken)) {
			return;
		}
		ClearPrint();
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
		g_loadingRate = 0;
		getData().m_scrollRate = Game::Config().m_scrollRate;

		m_asyncUpdater.reset(std::bind(&FileLoadScene::updateAsync, this));
		m_asyncUpdater.resume();
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
		m_view.update();
		m_asyncUpdater.resume();
	}

	void FileLoadScene::updateFadeIn([[maybe_unused]]double t)
	{
		m_view.update();
	}

	void FileLoadScene::updateFadeOut([[maybe_unused]] double t)
	{
		m_view.update();
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
		// マイグレーション処理
		co_await Thread::Task(MigrationSystem::Up);

		// ロード開始
		co_await Thread::Task(::LoadContentsData);

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
