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

    s3d::Array<MusicNotesIndex> NotesFinder::FindIndexes(const CTCFReader& ctcf)
    {
        auto& musics = Game::Musics();
        Array<MusicNotesIndex> candidate;
        for (const auto& m : musics) {
            for (const auto& notes : ctcf.select(m)) {
                candidate.push_back(notes.getMusicIndex());
            }
        }
        return candidate;
    }

    bool NotesFinder::HasNotes(const CTCFReader& ctcf)
    {
        auto& musics = Game::Musics();
        for (auto&& m : musics) {
            if (ctcf.hasNotes(m)) {
                return true;
            }
        }
        return false;
    }
    s3d::Optional<MusicNotesIndex> ct::NotesFinder::ChoiceIndex(const CTCFReader& ctcf)
    {
        Array<MusicNotesIndex> candidate = FindIndexes(ctcf);
        if (candidate.isEmpty()) {
            return none;
        }
        return candidate.choice();
    }
}
