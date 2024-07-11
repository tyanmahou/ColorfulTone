#pragma once
#include <Siv3D/String.hpp>
#include <Siv3D/StringView.hpp>

namespace ct
{
    class FileUtil
    {
    public:
        static s3d::String BaseName(s3d::FilePathView path);
    };
}