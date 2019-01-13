#include"KeyConfigScene.h"
#include"Fade.h"

#include"PlayKey.h"
#include"SceneInfo.h"
#include"Util.h"

namespace
{
	void InitTapSE(Config& config, String& configParm, const String& assetTag, const String& defaultSEPath)
	{
		config.add(L"なし", [=, &configParm]() {
			configParm = L"Resource/Sound/SE/none.mp3";
			SoundAsset::Unregister(assetTag);
			SoundAsset::Register(assetTag, L"Resource/Sound/SE/none.mp3", { L"System" });
		});
		config.add(L"デフォルト", [=, &configParm]() {
			configParm = defaultSEPath;
			SoundAsset::Unregister(assetTag);
			SoundAsset::Register(assetTag, defaultSEPath, { L"System" });
			SoundAsset(assetTag).playMulti();
		});

		for (const auto& path : Game::Instance()->m_tapSEPaths)
		{
			auto name = FileSystem::Relative(path, L"TapSE");
			config.add(name, [=, &configParm]() {
				configParm = path;
				SoundAsset::Unregister(assetTag);
				SoundAsset::Register(assetTag, path, { L"System" });
				SoundAsset(assetTag).playMulti();
			});
		}

		config.setDefault(L"デフォルト");
		if (configParm == L"Resource/Sound/SE/none.mp3")
			config.init(L"なし");
		else if (configParm == defaultSEPath)
			config.init(L"デフォルト");
		else
			config.init(FileSystem::Relative(configParm, L"TapSE"));
	}
	class TapSEConfig :public IConfigHierchy
	{
		enum Mode :int
		{
			Perfect,
			Great,
			Good, //コンフィグの数
			TOTAL_CONFIG //コンフィグの数
		};

	public:
		TapSEConfig()
		{
			m_configs.resize(TOTAL_CONFIG);
			m_configs[Perfect].setName(L"PERFECT");
			::InitTapSE(m_configs[Perfect], Game::Instance()->m_config.m_perfectSE, L"PERFECT", L"Resource/Sound/SE/tapP.wav");

			m_configs[Great].setName(L"GREAT");
			::InitTapSE(m_configs[Great], Game::Instance()->m_config.m_greatSE, L"GREAT", L"Resource/Sound/SE/tapGR.wav");

			m_configs[Good].setName(L"GOOD");
			::InitTapSE(m_configs[Good], Game::Instance()->m_config.m_goodSE, L"GOOD", L"Resource/Sound/SE/tapGD.wav");
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
			{ 1.1f, L"x1.1" },
			{ 1.2f, L"x1.2" },
			{ 1.3f, L"x1.3" },
			{ 1.4f, L"x1.4" },
			{ 1.5f, L"x1.5" },
			{ 1.6f, L"x1.6" },
			{ 1.7f, L"x1.7" },
			{ 1.8f, L"x1.8" },
			{ 1.9f, L"x1.9" },
			{ 2.0f, L"x2.0" },
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
		config.init(map.at(configParam));
	}
	class VolumeConfig :public IConfigHierchy
	{
		enum
		{
			BGMVolume,
			SEVolume,
			TOTAL_SIZE
		};
	public:
		VolumeConfig()
		{
			m_configs.resize(TOTAL_SIZE);
			m_configs[BGMVolume].setName(L"BGM");
			VolumeInit(m_configs[BGMVolume], SoundManager::BGM::SetVolume,
				Game::Instance()->m_config.m_bgmVolume);

			m_configs[SEVolume].setName(L"SE");
			VolumeInit(m_configs[SEVolume], SoundManager::SE::SetVolume,
				Game::Instance()->m_config.m_seVolume);
		}
	};
}

namespace
{
	//表示クリアレート
	void ClearRateInit(Config& config)
	{
		config.setName(L"表示するクリアレート");
		config.add(L"加算式", []() {Game::Instance()->m_config.m_isClearRateDownType = false; });
		config.add(L"減算式", []() {Game::Instance()->m_config.m_isClearRateDownType = true; });

		if (Game::Instance()->m_config.m_isClearRateDownType)
			config.init(L"減算式");
		else
			config.init(L"加算式");

	}
	//円形切り取りの初期化
	void CircleCutInit(Config& config)
	{
		config.setName(L"円形切り取り");
		config.add(L"ON", []() {Game::Instance()->m_config.m_isCirleCut = true; });
		config.add(L"OFF", []() {Game::Instance()->m_config.m_isCirleCut = false; });

		if (Game::Instance()->m_config.m_isCirleCut)
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
			config.add(pair.second, [&pair]() {Game::Instance()->m_config.m_playScale = pair.first; });
		}
		config.setDefault(L"x1.0");
		config.init(map.at(Game::Instance()->m_config.m_playScale));
	}
	void PlayBGInit(Config& config)
	{
		config.setName(L"背景設定");

		config.add(L"デフォルト", []() {Game::Instance()->m_config.m_bgType = BGType::Default; });
		config.add(L"ガウスぼかし", []() {Game::Instance()->m_config.m_bgType = BGType::Blur; });
		config.add(L"黒", []() {Game::Instance()->m_config.m_bgType = BGType::Black; });
		config.add(L"白", []() {Game::Instance()->m_config.m_bgType = BGType::White; });

		switch (Game::Instance()->m_config.m_bgType)
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
			Game::Instance()->m_config.m_styleType = PlayStyleType::Default; 
		});
		config.add(L"縦レーン", [] {
			Game::Instance()->m_config.m_styleType = PlayStyleType::Portrait;
		});

		switch (Game::Instance()->m_config.m_styleType)
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
		config.add(L"ON", []() {Game::Instance()->m_config.m_isSpectrum = true; });
		config.add(L"OFF", []() {Game::Instance()->m_config.m_isSpectrum = false; });

		if (Game::Instance()->m_config.m_isSpectrum)
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

		KeyConfig m_keyConfig;

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
	m_timer = 19200 * Sin(System::FrameCount() / 200.0);

	m_config.update();

	if (PlayKey::BigBack().clicked)
	{
		SoundManager::SE::Play(L"desisionLarge");
		changeScene(L"title", 3000);
	}
}

//--------------------------------------------------------------------------------
//関数：draw
//--------------------------------------------------------------------------------

void ConfigScene::draw()const
{

	static PixelShader ps(L"Shaders/mainBg.ps");
	static ConstantBuffer<Float4> cb;
	cb->set(static_cast<float>(m_timer), 0, 0, 0);
	Graphics2D::SetConstant(ShaderStage::Pixel, 1, cb);
	Graphics2D::BeginPS(ps);
	TextureAsset(L"keyconBG").draw();
	Graphics2D::EndPS();

	m_config.draw();

	SceneInfo::Draw(L"Enter:決定 BackSpace:戻る Esc:タイトルに戻る");
}

//--------------------------------------------------------------------------------
//関数：drawFadeIn
//--------------------------------------------------------------------------------
void ConfigScene::drawFadeIn(double t) const
{

	draw();
	FadeIn(Fade::SmoothCircle, t);

}

//--------------------------------------------------------------------------------
//関数：drawFadeOut
//--------------------------------------------------------------------------------
void ConfigScene::drawFadeOut(double t) const
{

	draw();
	FadeOut(Fade::SmoothCircle, t);

}
