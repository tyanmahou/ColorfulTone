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
            s3d::String detail;

            double time;

            double currentIndex = 0.0;
            double velocity = 0.0;

            bool isError = false;
        };
    public:
        void show(s3d::StringView message, s3d::StringView detail = U"");
        void error(s3d::StringView message, s3d::StringView detail = U"");
        void update(double dt);

        void draw() const;
    private:
        s3d::Array<Entry> m_entries;
    };
}