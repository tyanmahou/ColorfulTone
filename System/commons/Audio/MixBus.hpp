#pragma once
#include <Siv3D/MixBus.hpp>

namespace ct
{
    struct MixBusKind
    {
        inline static constexpr s3d::MixBus Bgm = s3d::MixBus::Index0;
        inline static constexpr s3d::MixBus Se = s3d::MixBus::Index1;
    };
}
