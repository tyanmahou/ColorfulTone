#include <scenes/utils/SharedLogic.hpp>
#include <utils/Input/InputUtl.hpp>
#include <core/Input/PlayKey.hpp>

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
}