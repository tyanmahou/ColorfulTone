#pragma once
#include <core/Data/MusicNotesIndex.hpp>
#include <core/Data/Genre/CTCFReader.hpp>

namespace ct
{
    class NotesFinder
    {
    public:
        static s3d::Optional<MusicNotesIndex> FindIndex(const s3d::String& notePath);

        static s3d::Array<MusicNotesIndex> FindIndexes(const CTCFReader& ctcf);
        static bool HasNotes(const CTCFReader& ctcf);


        static s3d::Optional<MusicNotesIndex> ChoiceIndex(const CTCFReader& ctcf);
    };
}