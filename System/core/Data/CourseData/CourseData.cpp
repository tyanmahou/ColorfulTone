#include <core/Data/CourseData/CourseData.hpp>
#include <core/Data/MusicData/MusicData.hpp>
#include <core/Data/Loader/ScoreLoader.hpp>
#include <commons/Game/Game.hpp>
#include <utils/File/FileUtil.hpp>
#include <Siv3D.hpp>
namespace ct
{
    size_t CourseData::Index = 0;

    CourseData::CourseData(const s3d::String& path):
        m_color(s3d::Palette::White)
    {
        m_index = Index++;
        this->load(path);
    }
    bool CourseData::load(const String& path)
    {
        INI ini(path);
        if (!ini)
            return false;

        m_fileName = FileUtil::BaseName(path);
        m_genre = FileSystem::FileName(FileSystem::ParentPath(path));

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

    void CourseData::saveScore(const CourseScore& score)const
    {
        CourseScoreLoader::Save(this->getScorePath(), score);
    }

    String CourseData::getScorePath() const
    {
        return U"UserData/CourseScore/" + m_genre + U"/" + m_fileName + U".bin";
    }

    Color CourseData::getStarColor() const
    {
        if (m_score.special == CourseSpecialResult::AP) {
            return Palette::Red;
        } else if (m_score.special == CourseSpecialResult::RankAAA) {
            return Palette::Yellow;
        }
        return Palette::White;
    }
}
