#include <utils/File/FileUtil.hpp>
#include <Siv3D.hpp>

namespace ct
{
    s3d::String FileUtil::BaseName(s3d::FilePathView path)
    {
        if (path.ends_with(U'/')) {
            path.remove_suffix(1);
        }
        return s3d::FileSystem::BaseName(path);
    }
    s3d::String FileUtil::FileName(s3d::FilePathView path)
    {
        if (path.ends_with(U'/')) {
            path.remove_suffix(1);
        }
        return s3d::FileSystem::FileName(path);
    }
    s3d::String FileUtil::ParentDirName(s3d::FilePathView path)
    {
        return FileUtil::FileName(FileSystem::ParentPath(path));
    }
}
