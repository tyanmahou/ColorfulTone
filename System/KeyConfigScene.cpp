#include "KeyConfigScene.h"
#include "Useful.hpp"
#include "SharedDraw.hpp"

#include"KeyConfigManager.hpp"

namespace
{
	void InitTapSE(Config& config)
	{
		for (const auto& se : Game::TapSEs())
		{
			config.add(se.getName(), [&]() {
				Game::Config().m_tapSE = se;
				se.apply();
				});
			config.init(Game::Config().m_tapSE.getName());
		}
	}
	class TapSEConfig :public IConfigHierchy
	{
		enum Mode :int
		{
			All,
			Perfect,
			Great,
			Good, //コンフィグの数
			TOTAL_CONFIG //コンフィグの数
		};
		float m_volume = 0.0f;
	public:
		TapSEConfig()
		{
			m_volume = SoundManager::BGM::GetVolume();
			SoundManager::BGM::SetVolume(s3d::Min(0.1f, m_volume));

			m_configs.resize(TOTAL_CONFIG);
			m_configs[All].setName(L"タップ音");
			::InitTapSE(m_configs[All]);
			m_configs[Perfect].setName(L"PERFECT 試聴");
			m_configs[Perfect].applyOnEnterd([]() {
				SoundManager::SE::Play(L"PERFECT");
				});

			m_configs[Great].setName(L"GREAT 試聴");
			m_configs[Great].applyOnEnterd([]() {
				SoundManager::SE::Play(L"GREAT");
				});

			m_configs[Good].setName(L"GOOD 試聴");
			m_configs[Good].applyOnEnterd([]() {
				SoundManager::SE::Play(L"GOOD");
				});
		}
		~TapSEConfig()
		{
			SoundManager::BGM::SetVolume(m_volume);
		}
	};
}

namespace
{
	//volume
	void VolumeInit(Config& config, std::function<void(float)> func, float& configParam, const String& default = L"x1.0")
	{
		static const std::map<float, String> map
		{
			{ 0.0f ,L"x0.0" },
			{ 0.1f ,L"x0.1" },
			{ 0.2f ,L"x0.2" },
			{ 0.3f ,L"x0.3" },
			{ 0.4f ,L"x0.4" },
			{ 0.5f ,L"x0.5" },
			{ 0.6f, L"x0.6" },
			{ 0.7f, L"x0.7" },
			{ 0.8f, L"x0.8" },
			{ 0.9f, L"x0.9" },
			{ 1.0f, L"x1.0" },
		};
		for (auto&& pair : map)
		{
			config.add(pair.second, [&pair, func, &configParam]()
				{
					configParam = pair.first;
					func(pair.first);
				});
		}
		config.setDefault(default);
		if (map.find(configParam) != map.end())
		{
			config.init(map.at(configParam));
		}
		else
		{
			config.init(default);
		}
	}
	class VolumeConfig :public IConfigHierchy
	{
		enum
		{
			BGMVolume,
			SEVolume,
			MasterVolume,
			TOTAL_SIZE
		};
	public:
		VolumeConfig()
		{
			m_configs.resize(TOTAL_SIZE);
			m_configs[BGMVolume].setName(L"BGM");
			VolumeInit(m_configs[BGMVolume], SoundManager::BGM::SetVolume,
				Game::Config().m_bgmVolume);

			m_configs[SEVolume].setName(L"SE");
			VolumeInit(m_configs[SEVolume], SoundManager::SE::SetVolume,
				Game::Config().m_seVolume);

			m_configs[MasterVolume].setName(L"Master");
			VolumeInit(m_configs[MasterVolume], MasterVoice::SetVolume,
				Game::Config().m_masterVolume);
		}
	};
}

namespace
{
	//表示クリアレート
	void ClearRateInit(Config& config)
	{
		config.setName(L"表示するクリアレート");
		config.add(L"加算式", []() {Game::Config().m_rateType = IndicateRate::Up; });
		config.add(L"減算式", []() {Game::Config().m_rateType = IndicateRate::Down; });
		config.add(L"ライフゲージ", []() {Game::Config().m_rateType = IndicateRate::Life; });

		if (Game::Config().m_rateType == IndicateRate::Up)
		{
			config.init(L"加算式");
		}
		else if (Game::Config().m_rateType == IndicateRate::Down)
		{
			config.init(L"減算式");
		}
		else if (Game::Config().m_rateType == IndicateRate::Life)
		{
			config.init(L"ライフゲージ");
		}
	}
	//円形切り取りの初期化
	void CircleCutInit(Config& config)
	{
		config.setName(L"円形切り取り");
		config.add(L"ON", []() {Game::Config().m_isCirleCut = true; });
		config.add(L"OFF", []() {Game::Config().m_isCirleCut = false; });

		if (Game::Config().m_isCirleCut)
			config.init(L"ON");
		else
			config.init(L"OFF");
	}
	//プレイスケール
	void PlayScaleInit(Config& config)
	{
		config.setName(L"プレイ画面の拡大率");
		static const std::map<float, String> map
		{
			{ 0.5f,L"x0.5" },
			{ 0.6f, L"x0.6" },
			{ 0.7f, L"x0.7" },
			{ 0.8f, L"x0.8" },
			{ 0.9f, L"x0.9" },
			{ 1.0f, L"x1.0" },
			{ 1.1f, L"x1.1" },
			{ 1.2f, L"x1.2" },
		};
		for (auto&& pair : map)
		{
			config.add(pair.second, [&pair]() {Game::Config().m_playScale = pair.first; });
		}
		config.setDefault(L"x1.0");
		config.init(map.at(Game::Config().m_playScale));
	}
	void TimingAdjustInit(Config& config)
	{
		config.setName(L"タイミング調整");
		for (int adjust : step(-10,10))
		{
			config.add(Format(adjust), [=]() {Game::Config().m_timingAdjust = adjust; });
		}
		config.setDefault(L"0");
		config.init(Format(Game::Config().m_timingAdjust));
	}
	void PlayBGInit(Config& config)
	{
		config.setName(L"背景設定");

		config.add(L"デフォルト", []() {Game::Config().m_bgType = BGType::Default; });
		config.add(L"ガウスぼかし", []() {Game::Config().m_bgType = BGType::Blur; });
		config.add(L"黒", []() {Game::Config().m_bgType = BGType::Black; });
		config.add(L"白", []() {Game::Config().m_bgType = BGType::White; });

		switch (Game::Config().m_bgType)
		{
		case BGType::Default: config.init(L"デフォルト");
			break;
		case BGType::Blur: config.init(L"ガウスぼかし");
			break;
		case BGType::Black: config.init(L"黒");
			break;
		case BGType::White: config.init(L"白");
			break;

		}
	}
	//style
	void PlayStyleInit(Config& config)
	{
		config.setName(L"プレイモード");

		config.add(L"通常モード", [] {
			Game::Config().m_styleType = PlayStyleType::Default;
			});
		config.add(L"縦レーン", [] {
			Game::Config().m_styleType = PlayStyleType::Portrait;
			});

		switch (Game::Config().m_styleType)
		{
		case PlayStyleType::Default: config.init(L"通常モード");
			break;
		case PlayStyleType::Portrait: config.init(L"縦レーン");
			break;
		}
	}
	//オーディオスペクトラムの初期化
	void IsSpectrumInit(Config& config)
	{
		config.setName(L"オーディオスペトラムの表示");
		config.add(L"ON", []() {Game::Config().m_isSpectrum = true; });
		config.add(L"OFF", []() {Game::Config().m_isSpectrum = false; });

		if (Game::Config().m_isSpectrum)
			config.init(L"ON");
		else
			config.init(L"OFF");
	}
	class PlayConfig :public IConfigHierchy
	{
		enum Mode
		{
			ClearRate,
			CircleCut,
			PlayScale,
			TimingAdjust,
			BGType,
			IsSpectrum,
			Style,
			TOTAL_CONFIG //コンフィグの数
		};
	public:
		PlayConfig()
		{
			m_configs.resize(TOTAL_CONFIG);
			::ClearRateInit(m_configs[ClearRate]);
			::CircleCutInit(m_configs[CircleCut]);
			::PlayScaleInit(m_configs[PlayScale]);
			::TimingAdjustInit(m_configs[TimingAdjust]);
			::PlayBGInit(m_configs[BGType]);
			::IsSpectrumInit(m_configs[IsSpectrum]);
			::PlayStyleInit(m_configs[Style]);
		}
	};

}
namespace
{


	class MainConfig :public IConfigHierchy
	{

		KeyConfigManager m_keyConfig;

		EasingController<double> m_keyConfigEasing;

		bool m_isKeyConfig = false;

		enum Mode
		{
			Play,
			Volume,
			TapSE,
			KeyConfig,
			TOTAL_CONFIG //コンフィグの数
		};
	public:
		MainConfig() :
			m_keyConfigEasing(600, 0, Easing::Linear, 500)
		{

			m_configs.resize(TOTAL_CONFIG);
			m_configs[Play].setName(L"プレイ画面");
			m_configs[Play].applyOnEnterd([this]() {
				this->changePush<::PlayConfig>();
				});
			m_configs[Volume].setName(L"音量");
			m_configs[Volume].applyOnEnterd([this]() {
				this->changePush<::VolumeConfig>();
				});

			m_configs[TapSE].setName(L"タップ音");
			m_configs[TapSE].applyOnEnterd([this]() {
				this->changePush<::TapSEConfig>();
				});
			m_configs[KeyConfig].setName(L"キーコンフィグ");
			m_configs[KeyConfig].applyOnEnterd([this]() {
				m_isKeyConfig = true;
				m_keyConfigEasing.start();
				});
		}

		bool update()override
		{
			if (m_keyConfigEasing.isActive())
			{
				return true;
			}

			if (m_isKeyConfig)
			{
				if (!m_keyConfig.update())
				{
					m_isKeyConfig = false;
					m_keyConfigEasing.start();
				}
			}
			else
			{
				IConfigHierchy::update();
			}
			return true;
		}

		void draw()const override
		{
			{
				util::Transformer2D t2d(Mat3x2::Translate(0, m_keyConfigEasing.easeInOut() - 600));

				IConfigHierchy::draw();
			}
			{
				util::Transformer2D t2d(Mat3x2::Translate(0, m_keyConfigEasing.easeInOut()));

				m_keyConfig.draw();
			}
		}
	};
}


//-----------------------------------------------------------------------------------------

ConfigScene::ConfigScene()
{

	m_config.changePush<::MainConfig>();

}

ConfigScene::~ConfigScene()
{}


//--------------------------------------------------------------------------------
//関数：update
//--------------------------------------------------------------------------------


void ConfigScene::update()
{
	m_config.update();

	if (PlayKey::BigBack().clicked)
	{
		SoundManager::SE::Play(L"desisionLarge");
		changeScene(SceneName::Title, 1000);
	}
}

//--------------------------------------------------------------------------------
//関数：draw
//--------------------------------------------------------------------------------

void ConfigScene::draw()const
{
	TextureAsset(L"canvasBg").draw();
	static SharedDraw::DrawBGLight lights;
	lights.update();
	lights.draw();

	m_config.draw();

	static const String title = L"CONFIG";
	SharedDraw::Sticky(&title, nullptr);

	SceneInfo::Draw(L"Enter:決定 BackSpace:戻る Esc:タイトルに戻る");
}

//--------------------------------------------------------------------------------
//関数：drawFadeIn
//--------------------------------------------------------------------------------
void ConfigScene::drawFadeIn(double t) const
{
	FadeIn(Fade::FlipPage, t, [this]() {draw(); }, true);
}

//--------------------------------------------------------------------------------
//関数：drawFadeOut
//--------------------------------------------------------------------------------
void ConfigScene::drawFadeOut(double t) const
{
	this->draw();
}
