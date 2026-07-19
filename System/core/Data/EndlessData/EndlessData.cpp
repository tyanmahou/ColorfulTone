#include <core/Data/EndlessData/EndlessData.hpp>
#include <utils/File/FileUtil.hpp>
#include <Siv3D.hpp>

namespace ct
{
    size_t EndlessData::Index = 0;


    EndlessData::EndlessData(const s3d::String& path):
        m_color(s3d::Palette::White)
    {
        m_index = Index++;
        this->load(path);
    }
    bool EndlessData::load(const s3d::String& path)
    {
        INI ini(path);
        if (!ini)
            return false;

        m_fileName = FileUtil::BaseName(path);
        m_score = {};

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
        return false;
    }
    void EndlessData::saveScore(const EndlessScore& score) const
    {
        // TODO
    }
    s3d::String EndlessData::getScorePath() const
    {
        return U"UserData/EndlessScore/" +  m_fileName + U".bin";
    }
    s3d::Texture ct::EndlessData::getTexture() const
    {
        if (!m_canPlay) {
            return s3d::TextureAsset(U"genre_missing");
        }
        return s3d::TextureAsset(U"genre_random");
    }
    const s3d::Color& EndlessData::getColor() const
    {
        return m_color;
    }
    const s3d::String& EndlessData::getTitle() const
    {
        return m_title;
    }
    s3d::String EndlessData::getDetail() const
    {
        if (m_canPlay) {
            return m_detail.value_or(U"？？？");
        } else {
            return U"条件にあう譜面がみつかりません\n" + m_detail.value_or(U"");
        }
    }
    const EndlessGaugeScore& EndlessData::getScore(LifeGaugeKind gauge) const
    {
        return m_score.gaugeData[static_cast<size_t>(gauge) - static_cast<size_t>(LifeGaugeKind::Min)];
    }
}
