#include"GameConfig.h"
#include"PlayKey.h"
#include"SoundManager.h"

GameConfig::GameConfig() :
	m_scrollRate(1.0f),
	m_bgmVolume(1.0),
	m_seVolume(1.0),
	m_masterVolume(1.0)
{
	m_red1 = Input::KeyF;
	m_red2 = Input::KeyD;
	m_red3 = Input::KeyS;

	m_blue1 = Input::KeySpace;
	m_blue2 = Input::KeyC;
	m_blue3 = Input::KeyM;

	m_yellow1 = Input::KeyJ;
	m_yellow2 = Input::KeyK;
	m_yellow3 = Input::KeyL;

}
namespace
{
	void InitKey(const INIReader& ini, const String& str, Key& key, const uint8 defaultCode=0)
	{
		const KeyDeviceType device = static_cast<KeyDeviceType>(ini.getOr<int>(L"Key." + str + L"_Device", 0));
		int code = ini.getOr<int>(L"Key." + str + L"_Code", -1);
		if (code == -1)
			code = defaultCode;
		const uint8 userIndex = ini.getOr<uint8>(L"Key." + str + L"_User", 0);
		if (device == KeyDeviceType::Gamepad)
		{
			key = Key(userIndex, code, Key::GamepadKeyCode{});
		}
		else if (device == KeyDeviceType::XInput)
		{
			key = Key(userIndex, code, Key::XInputKeyCode{});
		}
		else
		{
			key = Key(code, Key::VirtualKeyCode{});
		}
	}
}
void GameConfig::init()
{
	INIReader ini(L"config.ini");

	//F-70 GHIJ-74 KLMNOPQRS
	InitKey(ini, L"Red1", m_red1, 70);
	InitKey(ini, L"Red2", m_red2, 68);
	InitKey(ini, L"Red3", m_red3, 83);

	InitKey(ini, L"Blue1", m_blue1, 32);
	InitKey(ini, L"Blue2", m_blue2, 67);
	InitKey(ini, L"Blue3", m_blue3, 77);

	InitKey(ini, L"Yellow1", m_yellow1, 74);
	InitKey(ini, L"Yellow2", m_yellow2, 75);
	InitKey(ini, L"Yellow3", m_yellow3, 76);

	InitKey(ini, L"Up", m_up);
	InitKey(ini, L"Down", m_down);
	InitKey(ini, L"Left", m_left);
	InitKey(ini, L"Right", m_right);

	InitKey(ini, L"Start", m_start);
	InitKey(ini, L"SmallBack", m_smallBack);
	InitKey(ini, L"BigBack", m_bigBack);

	PlayKey::Init();

	m_scrollRate = ini.getOr<float>(L"Config.SpeedRate", 1.0f);

	m_isCirleCut = ini.getOr<bool>(L"Config.CirleCut", true);
	m_rateType = static_cast<IndicateRate>(ini.getOr<int>(L"Config.IndicateRate", 0));

	m_playScale = ini.getOr<float>(L"Config.PlayScale", 1.0f);
	m_timingAdjust = ini.getOr<int8>(L"Config.TimingAdjust", 0);

	const auto seName = ini.getOr<String>(L"Tap.Name", L"デフォルト");
	const auto perfectSE = ini.getOr<String>(L"Tap.Perfect", L"TapSE/デフォルト/tapP.mp3");
	const auto greatSE = ini.getOr<String>(L"Tap.Great", L"TapSE/デフォルト/tapGR.mp3");
	const auto goodSE = ini.getOr<String>(L"Tap.Good", L"TapSE/デフォルト/tapGD.mp3");

	m_tapSE = TapSE(seName, perfectSE, greatSE, goodSE);
	m_tapSE.apply();

	m_bgmVolume = ini.getOr<float>(L"Config.BGMVolume", 1.0f);
	m_seVolume = ini.getOr<float>(L"Config.SEVolume",1.0f);
	m_masterVolume = ini.getOr<float>(L"Config.MasterVolume", 1.0f);

	SoundManager::BGM::SetVolume(m_bgmVolume);
	SoundManager::SE::SetVolume(m_seVolume);
	MasterVoice::SetVolume(m_masterVolume);

	m_isSpectrum = ini.getOr<bool>(L"Config.IsSpectrum", true);
	m_bgType = static_cast<BGType>(ini.getOr<int>(L"Config.BGType", 0));
	m_styleType = static_cast<PlayStyleType>(ini.getOr<int>(L"Config.StyleType", 0));

}
namespace
{
	void SaveKey(INIWriter& ini, const String& str, Key& key)
	{
		ini.write(L"Key", str + L"_Device", static_cast<int>(key.device));
		ini.write(L"Key", str + L"_Code", key.code);
		ini.write(L"Key", str + L"_User", key.userIndex);
	}
}

void GameConfig::save()
{
	INIWriter ini(L"config.ini");

	//キーの保存
	SaveKey(ini, L"Red1", m_red1);
	SaveKey(ini, L"Red2", m_red2);
	SaveKey(ini, L"Red3", m_red3);
	SaveKey(ini, L"Blue1", m_blue1);
	SaveKey(ini, L"Blue2", m_blue2);
	SaveKey(ini, L"Blue3", m_blue3);
	SaveKey(ini, L"Yellow1", m_yellow1);
	SaveKey(ini, L"Yellow2", m_yellow2);
	SaveKey(ini, L"Yellow3", m_yellow3);

	SaveKey(ini, L"Up", m_up);
	SaveKey(ini, L"Down", m_down);
	SaveKey(ini, L"Left", m_left);
	SaveKey(ini, L"Right", m_right);

	SaveKey(ini, L"Start", m_start);
	SaveKey(ini, L"SmallBack", m_smallBack);
	SaveKey(ini, L"BigBack", m_bigBack);

	//タップ音の保存
	ini.write(L"Tap", L"Name", m_tapSE.getName());
	ini.write(L"Tap", L"Perfect", m_tapSE.getPerfectSE());
	ini.write(L"Tap", L"Great", m_tapSE.getGreatSE());
	ini.write(L"Tap", L"Good", m_tapSE.getGoodSE());

	//その他設定の保存
	ini.write(L"Config", L"SpeedRate", m_scrollRate);
	ini.write(L"Config", L"PlayScale", m_playScale);
	ini.write(L"Config", L"TimingAdjust", m_timingAdjust);
	ini.write(L"Config", L"CirleCut", m_isCirleCut);
	ini.write(L"Config", L"IndicateRate", static_cast<int>(m_rateType));

	ini.write(L"Config", L"BGMVolume", m_bgmVolume);
	ini.write(L"Config", L"SEVolume", m_seVolume);
	ini.write(L"Config", L"MasterVolume", m_masterVolume);

	ini.write(L"Config",L"IsSpectrum",m_isSpectrum);

	ini.write(L"Config", L"BGType", static_cast<int>(m_bgType));
	ini.write(L"Config", L"StyleType", static_cast<int>(m_styleType));
}