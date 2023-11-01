#pragma once

namespace ct
{
    class SoundManager
    {
    public:
        static void SetBgmVolume(double volume);
        static double GetBgmVolume();

        static void SetSeVolume(double volume);
        static double GetSeVolume();

        static void SetMasterVolume(double volume);
        static double GetMasterVolume();
    };
}