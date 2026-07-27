#include <core/Data/EndlessData/EndlessData.hpp>
#include <core/Data/Loader/ScoreLoader.hpp>
#include <core/Data/Finder/NotesFinder.hpp>
#include <utils/File/FileUtil.hpp>
#include <Siv3D.hpp>

namespace ct
{
    struct CandidateGroup
    {
        double weight = 0;
        CTCFReader condition;
    };
    class CTELeader
    {
    public:
        CTELeader()
        {
            groups.push_back({});
        }
        bool load(const FilePath& ctfolder)
        {
            TextReader reader(ctfolder);
            if (!reader) {
                return false;
            }
            String line;
            while (reader.readLine(line)) {
                s3d::String trimedLine = line.trimmed();
                if (trimedLine[0] == U'#') {
                    // メタ
                    this->parseMeta(trimedLine);
                    continue;
                } else if (trimedLine[0] == U'%') {
                    // %はコメント
                    continue;
                }
                else if (trimedLine == U"---") {
                    // next block
                    flashBuffer();
                    groups.push_back({});
                }
                else if (trimedLine.starts_with(U"WEIGHT") && trimedLine.includes(U':')) {
                    auto parses = trimedLine.split(U':');
                    if (parses.size() >= 2) {
                        groups.back().weight = s3d::Parse<double>(parses[1].trim());
                    }
                }
                else {
                    m_buffer += line;
                }
            }
            flashBuffer();
            groups.remove_if([](const CandidateGroup& group) {
                return group.weight <= 0 || !group.condition;
            });
            return groups.size() > 0;
        }
        s3d::String title = U"None";
        s3d::Optional<String> detail;
        s3d::Color color = s3d::Palette::White;
        s3d::Array<CandidateGroup> groups;
        s3d::int32 lv = 1;
        StarLv starLv = StarLv::None;
    private:
        void parseMeta(const String& option)
        {
            auto parses = option.split(U',');
            const auto& opt = parses[0].trim();
            if (opt == U"#TITLE" && parses.size() >= 2) {
                title = parses[1].trim();
            } else if (opt == U"#COLOR" && parses.size() >= 2) {
                color = Color(parses[1].trim());
            } else if (opt == U"#DETAIL" && parses.size() >= 2) {
                detail = parses[1].trim();
            } else if (opt == U"#LEVEL" && parses.size() >= 2) {
                lv = s3d::ParseOr<int32>(parses[1].trim(), 1);
                if (parses.size() >= 3) {
                    starLv = ParseStarLv(parses[2].trim());
                } else {
                    starLv = AutoStarLv(lv);
                }
            }
        }
        bool flashBuffer()
        {
            m_buffer.trim();
            if (m_buffer.isEmpty()) {
                return false;
            }
            groups.back().condition = CTCFReader(Arg::code = m_buffer);
            m_buffer.clear();
            return true;
        }
    private:
        String m_buffer;
    };
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
            m_index = Index++;
            m_fileName = FileUtil::BaseName(path);

            CTELeader reader{};
            if (!reader.load(path)) {
                return false;
            }


            //タイトル
            m_title = std::move(reader.title);
            // 詳細
            m_detail = std::move(reader.detail);
            // 色
            m_color = reader.color;
            m_lv = reader.lv;
            m_starLv = reader.starLv;

            m_candidateGroups = std::move(reader.groups);
            m_canPlay = m_candidateGroups.size() > 0 && m_candidateGroups.any([](const CandidateGroup& g) {
                return NotesFinder::HasNotes(g.condition);
            });
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
        s3d::int32 getLv() const
        {
            return m_lv;
        }
        StarLv getStarLv() const
        {
            return m_starLv;
        }
        const EndlessGaugeScore& getScore(LifeGaugeKind gauge) const
        {
            return m_score[gauge];
        }
        s3d::Array<EndlessCandidate> candidates() const
        {
            s3d::Array<EndlessCandidate> result;
            for (const auto& candidate : m_candidateGroups) {
                Array<MusicNotesIndex> indexes = NotesFinder::FindIndexes(candidate.condition);
                if (indexes.size() <= 0 || candidate.weight <= 0) {
                    continue;
                }
                result << EndlessCandidate{
                    .candidate = std::move(indexes),
                    .weight = candidate.weight,
                };
            }
            return result;
        }
    private:
        size_t m_index;	                //ID
        s3d::String m_title;            // タイトル
        s3d::Optional<String> m_detail;	// 詳細
        s3d::String m_fileName;         // ファイル名
        s3d::Color m_color;		        // 色
        s3d::int32 m_lv;                // レベル
        StarLv m_starLv = StarLv::None; // ★レベル

        EndlessScore m_score;

        bool m_canPlay = true;
        s3d::Array<CandidateGroup> m_candidateGroups;
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
    s3d::int32 EndlessData::getLv() const
    {
        return m_handle->getLv();
    }
    StarLv EndlessData::getStarLv() const
    {
        return m_handle->getStarLv();
    }
    const EndlessGaugeScore& EndlessData::getScore(LifeGaugeKind gauge) const
    {
        return m_handle->getScore(gauge);
    }
    s3d::Array<EndlessCandidate> EndlessData::candidates() const
    {
        return m_handle->candidates();
    }
}
