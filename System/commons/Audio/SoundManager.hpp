#pragma once
#include <Siv3D/AudioAsset.hpp>

namespace ct
{
    class SoundManager
    {
    public:
        static void SetBgmVolume(double volume);
        static double GetBgmVolume();

        static void PlaySe(s3d::AssetNameView name);
        static void SetSeVolume(double volume);
        static double GetSeVolume();

        static void SetMasterVolume(double volume);
        static double GetMasterVolume();
    };
}