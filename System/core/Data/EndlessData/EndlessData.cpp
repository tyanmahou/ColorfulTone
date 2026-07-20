#include <core/Data/EndlessData/EndlessData.hpp>
#include <core/Data/Loader/ScoreLoader.hpp>
#include <utils/File/FileUtil.hpp>
#include <Siv3D.hpp>

namespace ct
{
    class EndlessData::EndlessHandle
    {
    public:
        inline static size_t Index = 0;
    public:
        EndlessHandle():
            m_color(s3d::Palette::White)
        {
        }
        bool load(const s3d::String& path)
        {
            m_index = Index;
            Index++;

            INI ini(path);
            if (!ini)
                return false;

            m_fileName = FileUtil::BaseName(path);

            //タイトル
            m_title = ini.getOr<String>(U"Data.TITLE", U"None");
            // 詳細
            m_detail = ini.getOpt<String>(U"Data.DETAIL");
            // 色
            if (auto colorHex = ini.getOpt<String>(U"Data.COLOR")) {
                m_color = Color(*colorHex);
            }
            String randomCond = ini.getOr<String>(U"Data.CONDITION", U"");
            if (!randomCond.isEmpty()) {
                m_condition = CTCFReader(Arg::code = randomCond);
                m_canPlay = NotesFinder::HasNotes(m_condition);
            } else {
                m_canPlay = false;
            }

            // スコアロード
            m_score = EndlessScoreLoader::Load(this->getScorePath());
            return false;
        }

        void saveScore(LifeGaugeKind gauge, const EndlessGaugeScore& score)
        {
            m_score[gauge] = score;
            EndlessScoreLoader::Save(this->getScorePath(), m_score);
        }

        bool canPlay()const
        {
            return m_canPlay;
        }
        size_t getIndex()const
        {
            return m_index;
        }

        s3d::String getScorePath() const
        {
            return U"UserData/EndlessScore/" + m_fileName + U".bin";
        }

        s3d::Texture getTexture() const
        {
            if (!m_canPlay) {
                return s3d::TextureAsset(U"genre_missing");
            }
            return s3d::TextureAsset(U"genre_random");
        }
        const s3d::Color& getColor() const
        {
            return m_color;
        }
        const s3d::String& getTitle() const 
        {
            return m_title;
        }
        s3d::String getDetail() const
        {
            if (m_canPlay) {
                return m_detail.value_or(U"？？？");
            } else {
                return U"条件にあう譜面がみつかりません\n" + m_detail.value_or(U"");
            }
        }

        const EndlessGaugeScore& getScore(LifeGaugeKind gauge) const
        {
            return m_score[gauge];
        }
        s3d::Array<MusicNotesIndex> candidates() const
        {
            return NotesFinder::FindIndexes(m_condition);
        }
    private:
        size_t m_index;	//ID
        s3d::String m_title;	// タイトル
        s3d::Optional<String> m_detail;	// 詳細
        s3d::String m_fileName;
        s3d::Color m_color;		// 色
        EndlessScore m_score;

        bool m_canPlay = true;
        CTCFReader m_condition;
    };
    void EndlessData::ResetIndex()
    {
        EndlessHandle::Index = 0;
    }
    EndlessData::EndlessData(const s3d::String& path):
        m_handle(std::make_shared<EndlessHandle>())
    {
        this->load(path);
    }
    bool EndlessData::load(const s3d::String& path)
    {
        return m_handle->load(path);
    }
    void EndlessData::saveScore(LifeGaugeKind gauge, const EndlessGaugeScore& score) const
    {
        m_handle->saveScore(gauge, score);
    }
    bool EndlessData::canPlay() const
    {
        return m_handle->canPlay();
    }
    size_t EndlessData::getIndex()const
    {
        return m_handle->getIndex();
    }
    s3d::String EndlessData::getScorePath() const
    {
        return m_handle->getScorePath();
    }
    s3d::Texture ct::EndlessData::getTexture() const
    {
        return m_handle->getTexture();
    }
    const s3d::Color& EndlessData::getColor() const
    {
        return m_handle->getColor();
    }
    const s3d::String& EndlessData::getTitle() const
    {
        return m_handle->getTitle();
    }
    s3d::String EndlessData::getDetail() const
    {
        return m_handle->getDetail();
    }
    const EndlessGaugeScore& EndlessData::getScore(LifeGaugeKind gauge) const
    {
        return m_handle->getScore(gauge);
    }
    s3d::Array<MusicNotesIndex> ct::EndlessData::candidates() const
    {
        return m_handle->candidates();
    }
}
