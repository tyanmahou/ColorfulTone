#pragma once
#include <memory>

namespace ct
{
    class Object;
    class Bar;
    class Note;
    class LongNote;
    class TextObject;

    struct ObjectOrder
    {
        bool isActive = false;
        s3d::int64 judge = 0;
        s3d::int64 objType = 0;
        s3d::int64 kind = 0;
        s3d::int64 timming = 0;

        static ObjectOrder Calc(const Bar* obj, s3d::int64 sample);
        static ObjectOrder Calc(const Note* obj, s3d::int64 sample);
        static ObjectOrder Calc(const LongNote* obj, s3d::int64 sample);
        static ObjectOrder Calc(const TextObject* obj, s3d::int64 sample);

        std::strong_ordering operator <=> (const ObjectOrder& other) const;
    };
    struct ObjectSorter
    {
        s3d::int64 sample;
        bool operator()(const std::shared_ptr<Object>& a, const std::shared_ptr<Object>& b) const;
    };
}