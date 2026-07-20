#include <core/Data/CourseData/CourseData.hpp>
#include <core/Data/MusicData/MusicData.hpp>
#include <core/Data/Loader/ScoreLoader.hpp>
#include <commons/Game/Game.hpp>
#include <utils/File/FileUtil.hpp>
#include <Siv3D.hpp>

namespace ct
{
	class CourseData::CourseHandle
	{
	public:
		inline static size_t Index = 0;
	public:
		CourseHandle():
			m_color(s3d::Palette::White)
		{
		}
		bool load(const s3d::String& path)
		{
			m_index = Index++;

			INI ini(path);
			if (!ini)
				return false;

			m_fileName = FileUtil::BaseName(path);
			m_genre = FileUtil::ParentDirName(path);

			m_score = CourseScoreLoader::Load(this->getScorePath());

			//タイトル
			m_title = ini.getOr<String>(U"Data.TITLE", U"None");

			//譜面データのインデックス検索
			//譜面データ
			for (size_t i = 0; true; ++i) {
				Optional<String> detail = ini.getOpt<String>(Format(U"Course.DETAIL", i));
				// デフォルト
				String notePath = ini.get<String>(Format(U"Course.COURSE", i));
				if (!notePath.isEmpty()) {
					m_entries.push_back(CourceEntry::CreateDefault(notePath, detail));
					continue;
				}
				// ランダム
				String randomCond = ini.get<String>(Format(U"Course.RANDOM", i));
				if (!randomCond.isEmpty()) {
					m_entries.push_back(CourceEntry::CreateRandom(randomCond, detail));
					continue;
				}
				break;
			}

			m_canPlay = m_entries.all([](const CourceEntry& entry) {return entry.canPlay(); });

			if (auto colorHex = ini.getOpt<String>(U"Data.Color")) {
				m_color = Color(*colorHex);
			}
			return true;
		}

		void saveScore(const CourseScore& score)
		{
			m_score = score;
			CourseScoreLoader::Save(this->getScorePath(), score);
		}

		bool canPlay() const
		{
			return m_canPlay;
		}

		const s3d::Array<CourceEntry>& getEntries()const
		{
			return m_entries;
		}
		size_t getEntrySize() const
		{
			return m_entries.size();
		}
		const s3d::String& getTitle()const
		{
			return m_title;
		}
		const s3d::String& getFileName()const
		{
			return m_fileName;
		}
		const s3d::String& getGenre()const
		{
			return m_genre;
		}

		size_t getIndex()const
		{
			return m_index;
		}

		s3d::String getScorePath() const
		{
			return U"UserData/CourseScore/" + m_genre + U"/" + m_fileName + U".bin";
		}

		bool isClear() const
		{
			return m_score.isClear;
		}
		bool isMainClear() const
		{
			return m_score.isMainClear();
		}
		bool isLifeClear() const
		{
			return m_score.isLifeClear;
		}
		const CourseScore& getScore()const
		{
			return m_score;
		}
		void setScore(const CourseScore& score)
		{
			m_score = score;
		}

		s3d::Color getStarColor() const
		{
			if (m_score.special == CourseSpecialResult::AP) {
				return Palette::Red;
			} else if (m_score.special == CourseSpecialResult::RankAAA) {
				return Palette::Yellow;
			}
			return Palette::White;
		}

		const s3d::Color& getColor() const
		{
			return m_color;
		}

		const CourceEntry& operator[](size_t index) const
		{
			return m_entries[index];
		}

	private:
		s3d::Array<CourceEntry> m_entries;

		s3d::String m_title;	// コースタイトル
		s3d::String m_genre;	// ジャンル名
		size_t m_index;	//ID
		CourseScore m_score;

		s3d::String m_fileName;
		s3d::Color m_color;		// 色

		bool m_canPlay = true;
	};
	void CourseData::ResetIndex()
	{
		CourseHandle::Index = 0;
	}
	CourseData::CourseData(const s3d::String& path) :
		m_handle(std::make_shared<CourseHandle>())
	{
		this->load(path);
	}
    bool CourseData::load(const String& path)
    {
		return m_handle->load(path);
    }

    void CourseData::saveScore(const CourseScore& score)const
    {
		m_handle->saveScore(score);
    }

	bool CourseData::canPlay() const
	{
		return m_handle->canPlay();
	}

	const s3d::Array<CourceEntry>& CourseData::getEntries() const
	{
		return m_handle->getEntries();
	}

	size_t CourseData::getEntrySize() const
	{
		return m_handle->getEntrySize();
	}

	const s3d::String& CourseData::getTitle() const
	{
		return m_handle->getTitle();
	}

	const s3d::String& CourseData::getFileName() const
	{
		return m_handle->getFileName();
	}

	const s3d::String& CourseData::getGenre() const
	{
		return m_handle->getGenre();
	}

	size_t CourseData::getIndex() const
	{
		return m_handle->getIndex();
	}

    String CourseData::getScorePath() const
    {
		return m_handle->getScorePath();
    }

	bool CourseData::isClear() const
	{
		return m_handle->isClear();
	}

	bool CourseData::isMainClear() const
	{
		return m_handle->isMainClear();
	}

	bool CourseData::isLifeClear() const
	{
		return m_handle->isLifeClear();
	}

	const CourseScore& CourseData::getScore() const
	{
		return m_handle->getScore();
	}

    Color CourseData::getStarColor() const
    {
		return m_handle->getStarColor();
    }
	const s3d::Color& CourseData::getColor() const
	{
		return m_handle->getColor();
	}
	const CourceEntry& CourseData::operator[](size_t index) const
	{
		return getEntries()[index];
	}
}
