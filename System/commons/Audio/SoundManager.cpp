#include <commons/Audio/SoundManager.hpp>
#include <commons/Audio/MixBus.hpp>
#include <Siv3D.hpp>

namespace ct
{
    void SoundManager::SetBgmVolume(double volume)
    {
        GlobalAudio::BusSetVolume(MixBusKind::Bgm, volume);
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
    void SoundManager::SetMasterVolume(double volume)
    {
        GlobalAudio::SetVolume(volume);
    }
    double SoundManager::GetMasterVolume()
    {
        return GlobalAudio::GetVolume();
    }
}
