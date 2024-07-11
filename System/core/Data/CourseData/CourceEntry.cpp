#include <core/Data/CourseData/CourceEntry.hpp>
#include <Useful.hpp>
#include <Siv3D.hpp>

namespace
{
    using namespace ct;
    Optional<MusicNotesIndex> FindNotes(const String& notePath)
    {
        auto& musics = Game::Musics();

        const String fileName = FileUtil::BaseName(notePath);
        const String dirPath = notePath.removed(FileUtil::FileName(notePath));

        size_t musicIndex = 0;

        for (auto&& m : musics) {
            size_t notesIndex = 0;
            const String mName = m.getGenreName() + U"/" + m.getFileName() + U"/";
            if (mName == dirPath) {
                for (auto&& notes : m.getNotesData()) {
                    if (notes.getFileName() == fileName) {
                        return std::make_pair(musicIndex, notesIndex);
                    }
                    ++notesIndex;
                }
            }
            ++musicIndex;
        }
        return none;
    }
    bool FindhNotes(const CTCFReader& random)
    {
        auto& musics = Game::Musics();
        for (auto&& m : musics) {
            for (auto&& notes : m.getNotesData()) {
                if (random.expression(notes)) {
                    return true;
                }
            }
        }
        return false;
    }
    MusicNotesIndex ChoiceIndex(const CTCFReader& random)
    {
        auto& musics = Game::Musics();
        size_t musicIndex = 0;
        Array<MusicNotesIndex> candidate;

        for (const auto& m : musics) {
            size_t notesIndex = 0;
            for (const auto& notes : m.getNotesData()) {
                if (random.expression(notes)) {
                    candidate.push_back(std::make_pair(musicIndex, notesIndex));
                }
                ++notesIndex;
            }
            ++musicIndex;
        }
        if (candidate.isEmpty()) {
            return {0, 0};
        }
        return candidate.choice();
    }
}
namespace ct
{
    CourceEntry CourceEntry::CreateDefault(const s3d::String& path, const s3d::Optional<String>& detail)
    {
        CourceEntry ret;
        ret.m_kind = CourceEntryKind::Default;
        auto index = ::FindNotes(path);

        ret.m_data = CourceEntryDefault{
            .path = path,
            .index = index.value_or(MusicNotesIndex{})
        };
        ret.m_canPlay = index.has_value();
        ret.m_detail = detail;
        return ret;
    }
    CourceEntry CourceEntry::CreateRandom(const s3d::String& condition, const s3d::Optional<String>& detail)
    {
        CourceEntry ret;
        ret.m_kind = CourceEntryKind::Random;
        CTCFReader ctcf(Arg::code = condition);

        ret.m_data = CourceEntryRandom{
            .condition = ctcf
        };
        ret.m_canPlay = FindhNotes(ctcf);
        ret.m_detail = detail;
        return ret;
    }
    CourceSelectedNotes CourceEntry::choice() const
    {
        if (m_kind == CourceEntryKind::Default) {
            return {
                .index = std::get<CourceEntryDefault>(m_data).index,
                .isSecret = false,
            };
        }

        return {
            .index = ::ChoiceIndex(std::get<CourceEntryRandom>(m_data).condition),
            .isSecret = true,
        };
    }
    s3d::Texture CourceEntry::getJucketTexture() const
    {
        if (!m_canPlay) {
            return s3d::TextureAsset(U"genre_missing");
        }
        if (m_kind == CourceEntryKind::Default) {
            const auto& data = std::get<CourceEntryDefault>(m_data);
            return Game::Musics()[data.index.first].getTexture();
        }
        return s3d::TextureAsset(U"genre_random");
    }
    s3d::ColorF CourceEntry::getJucketColor() const
    {
        if (!m_canPlay) {
            return Palette::Black;
        }
        if (m_kind == CourceEntryKind::Default) {
            const auto& data = std::get<CourceEntryDefault>(m_data);
            return Game::Musics()[data.index.first][data.index.second].getColor();
        }
        return Palette::Magenta;
    }
    String CourceEntry::getJucketTitle() const
    {
        if (!m_canPlay) {
            return U"MISSING";
        }
        if (m_kind == CourceEntryKind::Default) {
            const auto& data = std::get<CourceEntryDefault>(m_data);
            return Game::Musics()[data.index.first].getMusicName();
        }
        return U"RANDOM";
    }
    String CourceEntry::getJucketDetail() const
    {
        if (m_kind == CourceEntryKind::Default) {
            const auto& data = std::get<CourceEntryDefault>(m_data);
            if (m_canPlay) {
                return Game::Musics()[data.index.first][data.index.second].getLevelNameAndLevel();
            } else {
                return U"譜面データが見つかりません\n" + data.path;
            }
        }
        if (m_canPlay) {
            return m_detail.value_or(U"？？？");
        } else {
            return U"条件にあう譜面がみつかりません\n" + m_detail.value_or(U"");
        }
    }
}
