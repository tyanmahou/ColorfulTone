#include <commons/Migration/MigrationUtil.hpp>
#include <Siv3D.hpp>


namespace ct
{
    void MigrationUtil::MoveScoreFolder()
    {
        if (FileSystem::Exists(U"Score/CourseScore")) {
            FileSystem::Rename(U"Score/CourseScore", U"UserData/CourseScore");
        }
        if (FileSystem::Exists(U"Score")) {
            FileSystem::Rename(U"Score", U"UserData/Score");
        }
        if (FileSystem::Exists(U"System")) {
            FileSystem::Rename(U"System", U"UserData/System");
        }
    }
}
