#include <scenes/utils/SharedLogic.hpp>
#include <utils/Input/InputUtl.hpp>
#include <core/Input/PlayKey.hpp>
#include <Siv3D.hpp>

namespace ct::SharedLogic
{
    s3d::int32 MoveSelectH()
    {
        if (InputUtil::AccelPressed(PlayKey::Left())) {
            return -1;
        }
        if (InputUtil::AccelPressed(PlayKey::Right())) {
            return 1;
        }
        return 0;
    }

    s3d::int32 MoveSelectV()
    {
        if (InputUtil::AccelPressed(PlayKey::Down())) {
            return -1;
        }
        if (InputUtil::AccelPressed(PlayKey::Up())) {
            return 1;
        }
        return 0;
    }
    s3d::int32 MoveSelectV(bool& playSe)
    {
        s3d::int32 timeMillisec = 5000;
        s3d::int32 waitMillisec = 500;
        if (InputUtil::AccelPressed(PlayKey::Down(), timeMillisec, waitMillisec, 12_fps)) {
            playSe = true;
            return -1;
        }
        if (InputUtil::AccelPressed(PlayKey::Up(), timeMillisec, waitMillisec, 12_fps)) {
            playSe = true;
            return 1;
        }
        return 0;
    }
}