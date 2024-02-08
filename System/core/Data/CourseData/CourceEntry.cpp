﻿#include <core/Data/CourseData/CourceEntry.hpp>
#include <Useful.hpp>
#include <Siv3D.hpp>
#include "CourceEntry.hpp"

namespace
{
    using namespace ct;
    Optional<MusicNotesIndex> FindNotes(const String& notePath)
    {
        auto& musics = Game::Musics();

        const String fileName = FileSystem::BaseName(notePath);
        const String dirPath = notePath.removed(FileSystem::FileName(notePath));

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
    CourceEntry CourceEntry::CreateDefault(const s3d::String& path)
    {
        CourceEntry ret;
        ret.m_kind = CourceEntryKind::Default;
        auto index = ::FindNotes(path);

        ret.m_data = CourceEntryDefault{
            .path = path,
            .index = index.value_or(MusicNotesIndex{})
        };
        ret.m_canPlay = index.has_value();
        return ret;
    }
    CourceEntry CourceEntry::CreateRandom(const s3d::String& condition)
    {
        CourceEntry ret;
        ret.m_kind = CourceEntryKind::Random;
        CTCFReader ctcf(Arg::code = condition);

        ret.m_data = CourceEntryRandom{
            .condition = ctcf
        };
        ret.m_canPlay = FindhNotes(ctcf);
        return ret;
    }
    MusicNotesIndex CourceEntry::choiceIndex() const
    {
        if (m_kind == CourceEntryKind::Default) {
            return std::get<CourceEntryDefault>(m_data).index;
        }

        return ::ChoiceIndex(std::get<CourceEntryRandom>(m_data).condition);
    }
    s3d::Texture CourceEntry::getJucketTexture() const
    {
        if (m_kind == CourceEntryKind::Default) {
            const auto& data = std::get<CourceEntryDefault>(m_data);
            if (m_canPlay) {
                return Game::Musics()[data.index.first].getTexture();
            }
        }
        return s3d::Texture();
    }
    s3d::ColorF CourceEntry::getJucketColor() const
    {
        if (m_kind == CourceEntryKind::Default) {
            const auto& data = std::get<CourceEntryDefault>(m_data);
            if (m_canPlay) {
                return Game::Musics()[data.index.first][data.index.second].getColor();
            }
        }
        return Palette::Gray;
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
            return U"？？？";
        } else {
            return U"条件にあう譜面がみつかりません";
        }
    }
}
