#pragma once
#include <scenes/Scene/FileLoad/FileLoadScene.hpp>
#include <Useful.hpp>
#include <utils/Thread/Task.hpp>
#include <utils/Coro/Fiber/FiberUtil.hpp>
#include <utils/Asset/SivAssetUtil.hpp>
#include <utils/File/FileUtil.hpp>

#include <core/Data/EndlessData/EndlessData.hpp>
#include <core/Data/Genre/CostumFolder.hpp>
#include <core/Data/TapSE/TapSE.hpp>
#include <commons/Migration/MigrationSystem.hpp>
#include <Siv3D.hpp>


namespace
{
	using namespace ct;
	class IContentsLoader
	{
	public:
		IContentsLoader() = default;
		virtual ~IContentsLoader() = default;

		virtual void setup() = 0;
		virtual bool load(const std::stop_token& stopToken) = 0;
		virtual size_t current() const = 0;
		virtual size_t total() const = 0;
		virtual double loadingFactor() const = 0;
	};
	// MusicDataのロード
	class MusicDataLoader : public IContentsLoader
	{
	public:
		void setup() override
		{
			Array<MusicData>& musics = Game::Musics();
			SivAssetUtil::UnregisterByTag<AudioAsset>(U"MusicData");
			musics.clear();

			GenreManager::Clear();
			m_genrePaths = FileSystem::DirectoryContents(U"Music", Recursive::No);
			MusicData::ResetIndex();

			m_current = 0;
			m_total = 0;
			for (const auto& gPath : m_genrePaths) {

				const auto musicPaths = FileSystem::DirectoryContents(gPath, Recursive::No);
				m_total += musicPaths.size();
			}

			musics.reserve(m_total);
		}
		bool load(const std::stop_token& stopToken) override
		{
			Array<MusicData>& musics = Game::Musics();

			// ジャンル予約用
			s3d::HashSet<int32> lvSet;
			s3d::HashSet<StarLv> starLvSet;
			s3d::HashSet<std::pair<String, bool>> folderSet;
			//ここから楽曲データ読み込み
			for (const auto& gPath : m_genrePaths) {
				bool foundMusic = false;
				const auto musicPaths = FileSystem::DirectoryContents(gPath, Recursive::No);
				const auto genreName = FileUtil::BaseName(gPath);

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
							foundMusic = true;
							break;
						}
					}
					++m_current;
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
				for (const auto& [folder, isOfficial] : folderSet) {
					genreRserves << GenreData::CreateFolder(folder, isOfficial);
				}
				//カスタムフォルダ読み込み
				genreRserves.append(::LoadCustomFolder(stopToken));

				genreRserves << GenreData::CreateAll();
				genreRserves << GenreData::CreateRandom();
				genreRserves << GenreData::CreateFavorite();

				GenreManager::Reflesh(genreRserves);
			}
			m_current = m_total;
			return true;
		}
		size_t current() const override
		{
			return m_current;
		}
		size_t total() const override
		{
			return m_total;
		}
		double loadingFactor() const override
		{
			return 1.0;
		}
	private:
		size_t m_current = 0;
		size_t m_total = 0;
		s3d::Array<s3d::FilePath> m_genrePaths;
	};
	// コースデータ読み込み
	class CourseDataLoader : public IContentsLoader
	{
	public:
		void setup() override
		{
			CourseGenreManager::Clear();
			CourseData::Index = 0;

			Array<CourseData>& courses = Game::Courses();
			courses.clear();

			m_genrePaths = FileSystem::DirectoryContents(U"Course", Recursive::No);
			//リサーブ
			m_current = 0;
			m_total = 0;
			for (const auto& gPath : m_genrePaths) {
				const auto ctcPaths = FileSystem::DirectoryContents(gPath, Recursive::No);
				for (const auto& path : ctcPaths) {
					if (FileSystem::Extension(path) == U"ctc") {
						++m_total;
					}
				}
			}
			courses.reserve(m_total);
		}
		bool load(const std::stop_token& stopToken) override
		{
			Array<CourseData>& courses = Game::Courses();
			for (const auto& gPath : m_genrePaths) {
				const auto genreName = FileUtil::BaseName(gPath);

				const auto ctcPaths = FileSystem::DirectoryContents(gPath, Recursive::No);
				for (const auto& path : ctcPaths) {
					if (stopToken.stop_requested()) {
						return false;
					}
					if (FileSystem::Extension(path) == U"ctc") {
						courses.emplace_back(path);
						++m_current;
					}
				}
				CourseGenreManager::Add(genreName);
			}
			m_current = m_total;
			return true;
		}
		size_t current() const override
		{
			return m_current;
		}
		size_t total() const override
		{
			return m_total;
		}
		double loadingFactor() const override
		{
			return 0.1;
		}
	private:
		size_t m_current = 0;
		size_t m_total = 0;
		s3d::Array<s3d::FilePath> m_genrePaths;
	};
	// エンドレスデータ読み込み
	class EndlessDataLoader : public IContentsLoader
	{
	public:
		void setup() override
		{
			EndlessData::ResetIndex();

			Array<EndlessData>& endless = Game::Endless();
			endless.clear();

			m_files = FileSystem::DirectoryContents(U"Endless", Recursive::No);

			m_current = 0;
			m_total = m_files.size();
			endless.reserve(m_total);
		}
		bool load(const std::stop_token& stopToken) override
		{
			Array<EndlessData>& endless = Game::Endless();
			for (const auto& filePath : m_files) {
				if (stopToken.stop_requested()) {
					return false;
				}
				if (FileSystem::Extension(filePath) == U"cte") {
					endless.emplace_back(filePath);
					++m_current;
				}
			}
			m_current = m_total;
			return true;
		}
		size_t current() const override
		{
			return m_current;
		}
		size_t total() const override
		{
			return m_total;
		}
		double loadingFactor() const override
		{
			return 0.1;
		}
	private:
		size_t m_current = 0;
		size_t m_total = 0;
		s3d::Array<s3d::FilePath> m_files;
	};

	// se読み込み
	class TapSELoader : public IContentsLoader
	{
	public:
		void setup() override
		{
			auto& tapSEs = Game::TapSEs();
			tapSEs.clear();

			m_files = FileSystem::DirectoryContents(U"TapSE", Recursive::No);

			m_current = 0;
			m_total = 0;
			for (auto&& rootFilePath : m_files) {
				if (FileSystem::IsDirectory(rootFilePath)) {
					++m_total;
				}
			}
			// noneの分+1
			tapSEs.reserve(m_total + 1);
		}
		bool load(const std::stop_token& stopToken) override
		{
			const FilePath nonePath = U"Resource/Sound/SE/none.mp3";
			auto& tapSEs = Game::TapSEs();
			tapSEs.emplace_back(U"なし", nonePath, nonePath, nonePath);

			for (auto&& rootFilePath : m_files) {
				if (stopToken.stop_requested()) {
					return false;
				}
				if (FileSystem::IsDirectory(rootFilePath)) {
					tapSEs.emplace_back(rootFilePath);
					++m_current;
				}
			}
			m_current = m_total;
			return true;
		}
		size_t current() const override
		{
			return m_current;
		}
		size_t total() const override
		{
			return m_total;
		}
		double loadingFactor() const override
		{
			return 0.1;
		}
	private:
		size_t m_current = 0;
		size_t m_total = 0;
		s3d::Array<s3d::FilePath> m_files;
	};
}
namespace ct
{
	class ContentLoadManager
	{
	public:
		template<class T, class... Args>
		void add(Args... args)
		{
			m_loaders.push_back(std::make_shared<T>(std::forward<Args>(args)...));
		}
		void setup() const
		{
			for (const auto& loader : m_loaders) {
				loader->setup();
			}
		}
		bool load(const std::stop_token& stopToken) const
		{
			for (const auto& loader : m_loaders) {
				if (!loader->load(stopToken)) {
					return false;
				}
			}
			return true;
		}
		double progress() const
		{
			double current = 0;
			double total = 0;
			for (const auto& loader : m_loaders) {
				double factor = loader->loadingFactor();
				current += factor * loader->current();
				total += factor * loader->total();
			}
			if (total <= 0) {
				return 0;
			}
			return current / total;
		}
	private:
		s3d::Array<std::shared_ptr<IContentsLoader>> m_loaders;
	};
    //--------------------------------------------------------------------------------
    //関数：コンストラクタ
    //--------------------------------------------------------------------------------
	FileLoadScene::FileLoadScene(const InitData& init) :
		ISceneBase(init),
		m_view(this),
		m_loader(std::make_shared<ContentLoadManager>())
	{
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
		return m_loader->progress();
	}

	Coro::Fiber<void> FileLoadScene::updateAsync()
	{
		// マイグレーション処理
		co_await Thread::Task(MigrationSystem::Up);

		// ロード開始
		{
			m_loader->add<MusicDataLoader>();
			m_loader->add<CourseDataLoader>();
			m_loader->add<EndlessDataLoader>();
			m_loader->add<TapSELoader>();
			m_loader->setup();
		}
		co_await Thread::Task(std::bind_front(&ContentLoadManager::load, m_loader.get()));
		ClearPrint();

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
