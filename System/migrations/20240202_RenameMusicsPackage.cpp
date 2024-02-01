#include <utils/Migration/Migration.hpp>
#include <commons/Migration/MigrationUtil.hpp>
#include <Siv3D.hpp>

namespace
{
    using namespace ct;

    MIGRATION(RenameMusicsPackage)
    {
        void up() const override
        {
            // ファイルパス変更
            MigrationUtil::RenameMusicFolder(U"NITMic", U"2016_01Summer");
            MigrationUtil::RenameMusicFolder(U"2017Spring", U"2017_01Spring");
            MigrationUtil::RenameMusicFolder(U"2017Summer", U"2017_02Summer");
            MigrationUtil::RenameMusicFolder(U"2017Autumn", U"2017_03Autumn");
            MigrationUtil::RenameMusicFolder(U"2018Summer", U"2018_01Summer");
        }
        void down() const override
        {
            // Not Impl
        }
    }_;
}
