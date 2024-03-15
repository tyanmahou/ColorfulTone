#include <scenes/Scene/MusicSelect/SelectMusicsInfo.hpp>

namespace ct
{
    SelectMusicsInfo& SelectMusicsInfo::Instance()
    {
        static SelectMusicsInfo selectInfo{};
        return selectInfo;
    }
}
