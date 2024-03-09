#include <core/Object/Bar/Bar.hpp>
#include <core/Play/PlayStyle/PlayStyle.hpp>

namespace ct
{
    bool Bar::update([[maybe_unused]] const PlaybackState& state)
    {
        return true;
    }
    void Bar::diffDraw(double count, double scrollRate)const
    {
        PlayStyle::Instance()->draw(*this, count, scrollRate);
    }
}