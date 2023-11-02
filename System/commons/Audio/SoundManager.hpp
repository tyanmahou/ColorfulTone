#pragma once
#include <Siv3D/AudioAsset.hpp>

namespace ct
{
    class SoundManager
    {
    public:
        static void PlayBgm(s3d::AssetNameView name, const s3d::Duration& fade = 0.0s);
        static void SetBgmVolume(double volume);
        static double GetBgmVolume();

        static void PlaySe(s3d::AssetNameView name);
        static void SetSeVolume(double volume);
        static double GetSeVolume();

        static void PlayInGameMusic(const s3d::Audio& audio);
        static void SetInGameMusicVolume(double volume);
        static double GetInGameMusicVolume();

        static void PlayInGameSe(s3d::AssetNameView name);
        static void SetInGameSeVolume(double volume);
        static double GetInGameSeVolume();

        static void SetMasterVolume(double volume);
        static double GetMasterVolume();
    };
}