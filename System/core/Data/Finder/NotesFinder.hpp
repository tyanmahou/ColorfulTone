#pragma once
#include <core/Data/Genre/CTCFReader.hpp>

namespace ct
{
    using MusicNotesIndex = std::pair<size_t, size_t>;

    class NotesFinder
    {
    public:
        static s3d::Optional<MusicNotesIndex> FindIndex(const s3d::String& notePath);

        static bool HasNotes(const CTCFReader& ctcf);


        static s3d::Optional<MusicNotesIndex> ChoiceIndex(const CTCFReader& ctcf);
    };
}