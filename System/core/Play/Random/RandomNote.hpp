#pragma once
#include <core/Play/Random/RandomNoteType.hpp>
#include <core/Types.hpp>
#include <utils/Singleton/Singleton.hpp>

namespace ct
{
    /// <summary>
    /// ランダム機能
    /// </summary>
    class RandomNote : protected Singleton<RandomNote>
    {
        friend class Singleton<RandomNote>;
    public:
        static void Init(RandomNoteType randomType);
        static NoteType Cast(NoteType type);
        static void ResetIgnore();
    private:
        void init(RandomNoteType randomType);
        NoteType cast(NoteType type);
        NoteType cast(NoteType type, RandomNoteType randomType);
        void resetIgnore();
    private:
        RandomNoteType m_type = RandomNoteType::None;
        RandomNoteType m_randomLane = RandomNoteType::None;
        s3d::int32 m_ignoreTypeBit = 0;
    };
}