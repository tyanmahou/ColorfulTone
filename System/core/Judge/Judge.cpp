#include <core/Judge/Judge.hpp>
#include <Siv3D.hpp>

namespace ct
{
    s3d::int64 JudgeRange(Judge judge)
    {
        // FPS 60想定
        return static_cast<s3d::int64>(44100 * static_cast<double>(judge) / 60.0);
    }
}
