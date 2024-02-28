#pragma once
#include <Siv3D/Array.hpp>
#include <Siv3D/String.hpp>

namespace ct
{
    class Notify
    {
        struct Entry
        {
            s3d::String message;

            double time;

            double currentIndex = 0.0;
            double velocity = 0.0;
        };
    public:
        void show(s3d::StringView message);
        void update(double dt);

        void draw() const;
    private:
        s3d::Array<Entry> m_entries;
    };
}