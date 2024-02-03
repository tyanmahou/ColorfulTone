#pragma once
#include <core/Play/PlayBG/BGType.hpp>
#include <core/Play/PlayBG/IPlayBG.hpp>
#include <memory>

namespace ct
{
    class PlayBGFactory
    {
    public:
        static std::shared_ptr<IPlayBG> CreateBG(BGType type = BGType::Default);
    };
}