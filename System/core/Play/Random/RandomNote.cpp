#include <core/Play/Random/RandomNote.hpp>
#include <Siv3D.hpp>

namespace ct
{
    void RandomNote::Init(RandomNoteType randomType)
    {
        Instance()->init(randomType);
    }
    NoteType RandomNote::Cast(NoteType type)
    {
        return Instance()->cast(type);
    }
    void RandomNote::init(RandomNoteType randomType)
    {
        m_type = randomType;
        if (randomType == RandomNoteType::Random) {
            m_randomLane = static_cast<RandomNoteType>(s3d::Random(0, 5));
        }
    }
    NoteType RandomNote::cast(NoteType type)
    {
        return this->cast(type, m_type);
    }
    NoteType RandomNote::cast(NoteType type, RandomNoteType randomType)
    {
        if (randomType == RandomNoteType::RBY) {
            // 123
            return type;
        }
        if (randomType == RandomNoteType::YBR) {
            // 321
            if (type == 1) return 3;
            if (type == 3) return 1;
            if (type == 4) return 6;
            if (type == 6) return 4;

            if (type == 11) return 13;
            if (type == 13) return 11;
            if (type == 14) return 16;
            if (type == 16) return 14;

            return type;
        }
        if (randomType == RandomNoteType::BYR) {
            // 231
            if (type == 1) return 3;
            if (type == 2) return 1;
            if (type == 3) return 2;
            if (type == 4) return 6;
            if (type == 5) return 4;
            if (type == 6) return 5;

            if (type == 11) return 13;
            if (type == 12) return 11;
            if (type == 13) return 12;
            if (type == 14) return 16;
            if (type == 15) return 14;
            if (type == 16) return 15;

            return type;
        }
        if (randomType == RandomNoteType::RYB) {
            // 132
            if (type == 3) return 2;
            if (type == 2) return 3;
            if (type == 1) return 1;
            if (type == 6) return 5;
            if (type == 5) return 6;
            if (type == 4) return 4;

            if (type == 13) return 12;
            if (type == 12) return 13;
            if (type == 11) return 11;
            if (type == 16) return 15;
            if (type == 15) return 16;
            if (type == 14) return 14;

            return type;
        }
        if (randomType == RandomNoteType::YRB) {
            // 312
            if (type == 1) return 2;
            if (type == 2) return 3;
            if (type == 3) return 1;
            if (type == 4) return 5;
            if (type == 5) return 6;
            if (type == 6) return 4;

            if (type == 11) return 12;
            if (type == 12) return 13;
            if (type == 13) return 11;
            if (type == 14) return 15;
            if (type == 15) return 16;
            if (type == 16) return 14;
        }
        if (randomType == RandomNoteType::BRY) {
            // 213
            if (type == 3) return 3;
            if (type == 2) return 1;
            if (type == 1) return 2;
            if (type == 6) return 6;
            if (type == 5) return 4;
            if (type == 4) return 5;

            if (type == 13) return 13;
            if (type == 12) return 11;
            if (type == 11) return 12;
            if (type == 16) return 16;
            if (type == 15) return 14;
            if (type == 14) return 15;
        }
        if (randomType == RandomNoteType::Random) {
            // ランダム
            return cast(type, m_randomLane);
        }
        //if (randomType == RandomNoteType::SRandom) {
        //    // Sランダム
        //    // TODO
        //}

        return type;
    }
}
