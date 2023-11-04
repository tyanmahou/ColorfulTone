#include <commons/Audio/SoundManager.hpp>
#include <commons/Audio/MixBus.hpp>
#include <Siv3D.hpp>
#include "SoundManager.hpp"

namespace ct
{
    void SoundManager::PlayBgm(s3d::AssetNameView name, const s3d::Duration& fade)
    {
        AudioAsset(name).play(MixBusKind::Bgm, fade);
    }
    void SoundManager::SetBgmVolume(double volume)
    {
        GlobalAudio::BusSetVolume(MixBusKind::Bgm, volume);
    }

    void SoundManager::FadeBgmVolume(double volume, const s3d::Duration& fade)
    {
        GlobalAudio::BusFadeVolume(MixBusKind::Bgm, volume, fade);
    }

    double SoundManager::GetBgmVolume()
    {
        return GlobalAudio::BusGetVolume(MixBusKind::Bgm);
    }
    void SoundManager::PlaySe(s3d::AssetNameView name)
    {
        AudioAsset(name).playOneShot(MixBusKind::Se, 1.0);
    }
    void SoundManager::SetSeVolume(double volume)
    {
        GlobalAudio::BusSetVolume(MixBusKind::Se, volume);
    }
    double SoundManager::GetSeVolume()
    {
        return GlobalAudio::BusGetVolume(MixBusKind::Se);
    }
    void SoundManager::PlayInGameMusic(const s3d::Audio& audio)
    {
        audio.play(MixBusKind::InGameMusic);
    }
    void SoundManager::SetInGameMusicVolume(double volume)
    {
        GlobalAudio::BusSetVolume(MixBusKind::InGameMusic, volume);
    }
    double SoundManager::GetInGameMusicVolume()
    {
        return GlobalAudio::BusGetVolume(MixBusKind::InGameMusic);
    }
    void SoundManager::PlayInGameSe(s3d::AssetNameView name)
    {
        AudioAsset(name).playOneShot(MixBusKind::InGameSe, 1.0);
    }
    void SoundManager::SetInGameSeVolume(double volume)
    {
        GlobalAudio::BusSetVolume(MixBusKind::InGameSe, volume);
    }
    double SoundManager::GetInGameSeVolume()
    {
        return GlobalAudio::BusGetVolume(MixBusKind::InGameSe);
    }
    void SoundManager::SetMasterVolume(double volume)
    {
        GlobalAudio::SetVolume(volume);
    }
    double SoundManager::GetMasterVolume()
    {
        return GlobalAudio::GetVolume();
    }
}
