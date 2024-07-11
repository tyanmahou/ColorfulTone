#include <utils/File/FileUtil.hpp>
#include <Siv3D.hpp>

namespace ct
{
    s3d::String FileUtil::BaseName(s3d::FilePathView path)
    {
        if (path.ends_with(U"/")) {
            return s3d::FileSystem::BaseName(path.substr(0, path.length() - 1));
        }
        return s3d::FileSystem::BaseName(path);
    }
}
