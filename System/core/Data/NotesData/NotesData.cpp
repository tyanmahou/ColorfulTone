#include <core/Data/NotesData/NotesData.hpp>
#include <core/Data/Genre/GenreManager.hpp>
#include <core/Data/Loader/ScoreLoader.hpp>
#include <Siv3D.hpp>
#include <Useful.hpp>
#include "NotesData.hpp"

namespace ct
{
    class NotesData::Handle
    {
    public:
        Handle()
        {}
        bool load(
            const std::shared_ptr<MusicHandle>& pMusic,
            const s3d::String& dirPath,
            const s3d::String& filePath,
            size_t index
        )
        {
            m_fileName = filePath.removed(U".csv");
            m_pMusic = pMusic;
            m_index = index;

            //セーブデータの読み込み
            m_score = ScoreLoader::Load(this->getScorePath());

            FilePath path = dirPath + filePath;
            m_filePath = path;
            if (!m_sheet.load(path)) {
                return false;
            }

            int32 lv = m_sheet.getLv();
            StarLv starLv = m_sheet.getStarLv();
            if (starLv != StarLv::None) {
                GenreManager::Add(GenreType::StarLv, Format(U"LEVEL:", ToStr(starLv)), [starLv](const MusicData& music)->bool {
                    return music.getNotesData().any([starLv](const NotesData& notes) {return notes.getStarLv() == starLv; });
                    });
            } else {
                GenreManager::Add(GenreType::Lv, Format(U"LEVEL:", lv), [lv](const MusicData& music)->bool {
                    return music.getNotesData().any([lv](const NotesData& notes) {return notes.getLevel() == lv; });
                    }, lv);
            }
            return true;
        }
        const SheetMusic& getSheet() const
        {
            return m_sheet;
        }
        const s3d::String& getFileName()const { return m_fileName; }
        const s3d::FilePath& getFilePath() const { return m_filePath; }
        const ScoreModel& getScore()const { return m_score; }
        void setScore(const ScoreModel& newScore)
        {
            m_score = newScore;
        }

        MusicData getMusic() const
        {
            return MusicData(m_pMusic.lock());
        }

        size_t getIndex()const
        {
            return m_index;
        }

        s3d::String getScorePath() const
        {
            MusicData music = getMusic();
            return U"UserData/Score/"
                + music.getGenreName() + U"/"
                + music.getFileName() + U"/"
                + this->m_fileName
                + U".bin";
        }

        void saveScore(const ScoreModel& score) const
        {
            ScoreLoader::Save(this->getScorePath(), score);
        }
        void reload()
        {
            m_sheet = SheetMusic();
            m_sheet.load(m_filePath);
        }
    private:
        SheetMusic m_sheet;                  // 楽譜データ
        s3d::String m_fileName;				 // 譜面ファイルの名前(拡張子を含まない)
        s3d::FilePath m_filePath;            // ファイルパス
        ScoreModel m_score;					 // スコア
        std::weak_ptr<MusicHandle> m_pMusic; // 曲情報
        size_t m_index;					     // ID
    };
    NotesData::NotesData(
        const std::shared_ptr<MusicHandle>& pMusic,
        const String& dirPath,
        const String& filePath,
        size_t index
    ) :
        m_handle(std::make_shared<Handle>())
    {
        m_handle->load(pMusic, dirPath, filePath, index);
    }

    size_t NotesData::getIndex() const
    {
        return m_handle->getIndex();
    }

    s3d::int32 NotesData::getLevel() const
    {
        return getSheet().getLv();
    }

    const s3d::String& NotesData::getLevelName() const
    {
        return getSheet().getLevelName();
    }

    StarLv NotesData::getStarLv() const
    {
        return getSheet().getStarLv();
    }

    s3d::String NotesData::getLevelWithStar() const
    {
        return getSheet().getLevelWithStar();
    }

    const s3d::String NotesData::getLevelNameAndLevel() const
    {
        return getSheet().getLevelNameAndLevel();
    }

    const s3d::String& NotesData::getNotesArtistName() const
    {
        return getSheet().getNotesArtistName();
    }

    const s3d::String& NotesData::getFileName() const
    {
        return m_handle->getFileName();
    }

    const s3d::FilePath& NotesData::getFilePath() const
    {
        return m_handle->getFilePath();
    }

    s3d::uint32 NotesData::getTotalNotes() const
    {
        return getSheet().getTotalNotes();
    }

    const s3d::Color& NotesData::getColor() const
    {
        return getSheet().getColor();
    }

    const SheetMusic& NotesData::getSheet() const
    {
        return m_handle->getSheet();
    }
    const ScoreModel& NotesData::getScore() const
    {
        return m_handle->getScore();
    }
    void NotesData::setScore(const ScoreModel& newScore)
    {
        return m_handle->setScore(newScore);
    }
    void NotesData::saveScore(const ScoreModel& score) const
    {
        return m_handle->saveScore(score);
    }
    MusicData NotesData::getMusic() const
    {
        return m_handle->getMusic();
    }
    void NotesData::reload()
    {
        m_handle->reload();
    }
}
