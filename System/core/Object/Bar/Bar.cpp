#include <core/Object/Bar/Bar.hpp>
#include <core/Play/PlayStyle/PlayStyle.hpp>

namespace ct
{
    bool Bar::update([[maybe_unused]] double nowCount, [[maybe_unused]] double countPerFrame)
    {
        return true;
    }
    void Bar::diffDraw(double count, double scrollRate)const
    {
        PlayStyle::Instance()->draw(*this, count, scrollRate);
    }
}