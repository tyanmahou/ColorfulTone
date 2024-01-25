#include <commons/Game/GameConfig.hpp>
#include <commons/Audio/SoundManager.hpp>
#include <core/Input/PlayKey.hpp>
#include <Siv3D.hpp>

namespace
{
    void InitKey(const INI& ini, const String& str, Input& key, const uint8 defaultCode = 0)
    {
        // NOTE: +1は旧Siv3Dとの互換性でする
        const InputDeviceType device = static_cast<InputDeviceType>(ini.getOr<int>(U"Key." + str + U"_Device", 0) + 1);
        uint8 code = ini.getOr<uint8>(U"Key." + str + U"_Code", -1);
        if (code == -1) {
            code = defaultCode;
        }
        const uint8 userIndex = ini.getOr<uint8>(U"Key." + str + U"_User", 0);
        key = Input(device, code, userIndex);
    }

    void SaveKey(INI& ini, const String& str, Input& key)
    {
        // NOTE: -1は旧Siv3Dとの互換性でする
        ini.write(U"Key", str + U"_Device", static_cast<int>(key.deviceType())-1);
        ini.write(U"Key", str + U"_Code", key.code());
        ini.write(U"Key", str + U"_User", key.playerIndex());
    }
}
namespace  ct
{
    GameConfig::GameConfig() :
        m_scrollRate(1.0f),
        m_bgmVolume(1.0),
        m_seVolume(1.0),
        m_inGameMusicVolume(1.0),
        m_inGameSeVolume(1.0),
        m_masterVolume(1.0)
    {
        m_red1 = KeyF;
        m_red2 = KeyD;
        m_red3 = KeyS;

        m_blue1 = KeySpace;
        m_blue2 = KeyC;
        m_blue3 = KeyM;

        m_yellow1 = KeyJ;
        m_yellow2 = KeyK;
        m_yellow3 = KeyL;
    }
    void GameConfig::init()
    {
        INI ini(U"config.ini");

        //F-70 GHIJ-74 KLMNOPQRS
        InitKey(ini, U"Red1", m_red1, 70);
        InitKey(ini, U"Red2", m_red2, 68);
        InitKey(ini, U"Red3", m_red3, 83);

        InitKey(ini, U"Blue1", m_blue1, 32);
        InitKey(ini, U"Blue2", m_blue2, 67);
        InitKey(ini, U"Blue3", m_blue3, 77);

        InitKey(ini, U"Yellow1", m_yellow1, 74);
        InitKey(ini, U"Yellow2", m_yellow2, 75);
        InitKey(ini, U"Yellow3", m_yellow3, 76);

        InitKey(ini, U"Up", m_up);
        InitKey(ini, U"Down", m_down);
        InitKey(ini, U"Left", m_left);
        InitKey(ini, U"Right", m_right);

        InitKey(ini, U"Start", m_start);
        InitKey(ini, U"SmallBack", m_smallBack);
        InitKey(ini, U"BigBack", m_bigBack);

        PlayKey::Init();

        m_scrollRate = ini.getOr<double>(U"Config.SpeedRate", 1.0);

        m_isCirleCut = ini.getOr<bool>(U"Config.CirleCut", true);
        m_rateType = static_cast<IndicateRate>(ini.getOr<int32>(U"Config.IndicateRate", 0));

        m_playScale = ini.getOr<double>(U"Config.PlayScale", 1.0);
        m_timingAdjust = ini.getOr<int8>(U"Config.TimingAdjust", 0);

        const auto seName = ini.getOr<String>(U"Tap.Name", U"デフォルト");
        const auto perfectSE = ini.getOr<String>(U"Tap.Perfect", U"TapSE/デフォルト/tapP.mp3");
        const auto greatSE = ini.getOr<String>(U"Tap.Great", U"TapSE/デフォルト/tapGR.mp3");
        const auto goodSE = ini.getOr<String>(U"Tap.Good", U"TapSE/デフォルト/tapGD.mp3");

        m_tapSE = TapSE(seName, perfectSE, greatSE, goodSE);
        m_tapSE.apply();

        m_bgmVolume = ini.getOr<double>(U"Config.BGMVolume", 1.0);
        m_seVolume = ini.getOr<double>(U"Config.SEVolume", 1.0);
        m_inGameMusicVolume = ini.getOr<double>(U"Config.InGameMusicVolume", 1.0);
        m_inGameSeVolume = ini.getOr<double>(U"Config.InGameSEVolume", 1.0);
        m_masterVolume = ini.getOr<double>(U"Config.MasterVolume", 1.0);

        SoundManager::SetBgmVolume(m_bgmVolume);
        SoundManager::SetSeVolume(m_seVolume);
        SoundManager::SetInGameMusicVolume(m_inGameMusicVolume);
        SoundManager::SetInGameSeVolume(m_inGameSeVolume);
        SoundManager::SetMasterVolume(m_masterVolume);

        m_isSpectrum = ini.getOr<bool>(U"Config.IsSpectrum", true);
        m_bgType = static_cast<BGType>(ini.getOr<int32>(U"Config.BGType", 0));
        m_styleType = static_cast<PlayStyleType>(ini.getOr<int32>(U"Config.StyleType", 0));
        m_bgBrightness = ini.getOr<int8>(U"Config.BGBrightness", 10);
    }

    void GameConfig::save()
    {
        INI ini(U"config.ini");

        //キーの保存
        SaveKey(ini, U"Red1", m_red1);
        SaveKey(ini, U"Red2", m_red2);
        SaveKey(ini, U"Red3", m_red3);
        SaveKey(ini, U"Blue1", m_blue1);
        SaveKey(ini, U"Blue2", m_blue2);
        SaveKey(ini, U"Blue3", m_blue3);
        SaveKey(ini, U"Yellow1", m_yellow1);
        SaveKey(ini, U"Yellow2", m_yellow2);
        SaveKey(ini, U"Yellow3", m_yellow3);

        SaveKey(ini, U"Up", m_up);
        SaveKey(ini, U"Down", m_down);
        SaveKey(ini, U"Left", m_left);
        SaveKey(ini, U"Right", m_right);

        SaveKey(ini, U"Start", m_start);
        SaveKey(ini, U"SmallBack", m_smallBack);
        SaveKey(ini, U"BigBack", m_bigBack);

        //タップ音の保存
        ini.write(U"Tap", U"Name", m_tapSE.getName());
        ini.write(U"Tap", U"Perfect", m_tapSE.getPerfectSE());
        ini.write(U"Tap", U"Great", m_tapSE.getGreatSE());
        ini.write(U"Tap", U"Good", m_tapSE.getGoodSE());

        //その他設定の保存
        ini.write(U"Config", U"SpeedRate", m_scrollRate);
        ini.write(U"Config", U"PlayScale", m_playScale);
        ini.write(U"Config", U"TimingAdjust", m_timingAdjust);
        ini.write(U"Config", U"CirleCut", m_isCirleCut);
        ini.write(U"Config", U"IndicateRate", static_cast<int32>(m_rateType));

        ini.write(U"Config", U"BGMVolume", m_bgmVolume);
        ini.write(U"Config", U"SEVolume", m_seVolume);
        ini.write(U"Config", U"InGameMusicVolume", m_inGameMusicVolume);
        ini.write(U"Config", U"InGameSEVolume", m_inGameSeVolume);
        ini.write(U"Config", U"MasterVolume", m_masterVolume);

        ini.write(U"Config", U"IsSpectrum", m_isSpectrum);

        ini.write(U"Config", U"BGType", static_cast<int32>(m_bgType));
        ini.write(U"Config", U"StyleType", static_cast<int32>(m_styleType));
        ini.write(U"Config", U"BGBrightness", m_bgBrightness);

        ini.save(U"config.ini");
    }
}