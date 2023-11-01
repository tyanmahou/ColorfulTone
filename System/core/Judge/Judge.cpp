#include <core/Judge/Judge.hpp>
#include <Siv3D.hpp>

namespace ct
{
    double JudgeRange(double countPerFrame, Judge judge)
    {
        return Abs(countPerFrame) * static_cast<double>(judge);
    }
}
