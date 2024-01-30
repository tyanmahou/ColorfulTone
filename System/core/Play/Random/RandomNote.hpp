#pragma once
#include <core/Play/Random/RandomNoteType.hpp>
#include <core/Types.hpp>
#include <utils/Singleton/DynamicSingleton.hpp>

namespace ct
{
    /// <summary>
    /// ランダム機能
    /// </summary>
    class RandomNote : protected DynamicSingleton<RandomNote>
    {
        friend class  DynamicSingleton<RandomNote>;
    public:
        static void Init(RandomNoteType randomType);
        static NoteType Cast(NoteType type);
    private:
        void init(RandomNoteType randomType);
        NoteType cast(NoteType type);
        NoteType cast(NoteType type, RandomNoteType randomType);
    private:
        RandomNoteType m_type = RandomNoteType::None;
        RandomNoteType m_randomLane = RandomNoteType::None;
    };
}