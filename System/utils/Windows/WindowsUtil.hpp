#pragma once
#include <Siv3D/StringView.hpp>

namespace ct
{
    class WindowsUtil
    {
    public:
        static bool OpenExplore(s3d::FilePathView path);
    };
}