#include <core/Data/Finder/NotesFinder.hpp>
#include <Useful.hpp>
#include <Siv3D.hpp>

namespace ct
{
    s3d::Optional<MusicNotesIndex> NotesFinder::FindIndex(const s3d::String& notePath)
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

    bool NotesFinder::HasNotes(const CTCFReader& ctcf)
    {
        auto& musics = Game::Musics();
        for (auto&& m : musics) {
            for (auto&& notes : m.getNotesData()) {
                if (ctcf.expression(notes)) {
                    return true;
                }
            }
        }
        return false;
    }
    s3d::Optional<MusicNotesIndex> ct::NotesFinder::ChoiceIndex(const CTCFReader& ctcf)
    {
        auto& musics = Game::Musics();
        size_t musicIndex = 0;
        Array<MusicNotesIndex> candidate;

        for (const auto& m : musics) {
            size_t notesIndex = 0;
            for (const auto& notes : m.getNotesData()) {
                if (ctcf.expression(notes)) {
                    candidate.push_back(std::make_pair(musicIndex, notesIndex));
                }
                ++notesIndex;
            }
            ++musicIndex;
        }
        if (candidate.isEmpty()) {
            return none;
        }
        return candidate.choice();
    }
}
