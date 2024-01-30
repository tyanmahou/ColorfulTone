#pragma once
#include <core/Play/Random/RandomNoteType.hpp>
#include <core/Types.hpp>
#include <utils/Singleton/DynamicSingleton.hpp>

namespace ct
{
    class RandomNote : public DynamicSingleton<RandomNote>
    {
        friend class  DynamicSingleton<RandomNote>;
    public:
    private:
        void init(RandomNoteType randomType);
        NoteType cast(NoteType type, RandomNoteType randomType);

        RandomNoteType m_randomLane;
    };
}