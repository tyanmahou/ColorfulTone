#include "CompatibilityUtil.hpp"
#include "Useful.hpp"

void CompatibilityUtil::MoveScoreFolder()
{
    if (FileSystem::Exists(L"Score/CourseScore")) {
        FileSystem::Move(L"Score/CourseScore", L"UserData/CourseScore");
    }
    if (FileSystem::Exists(L"Score")) {
        FileSystem::Move(L"Score", L"UserData/Score");
    }
    if (FileSystem::Exists(L"System")) {
        FileSystem::Move(L"System", L"UserData/System");
    }
}
