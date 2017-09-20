#include"KeyConfigScene.h"
#include"Fade.h"

#include"PlayKey.h"
#include"SceneInfo.h"

namespace
{
	//表示クリアレート
	void ClearRateInit(Config& config)
	{
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

}








ConfigScene::ConfigScene() :
	m_keyConfigEasing(600, 0, Easing::Linear, 500),
	m_mode(Mode::ClearRate),
	m_font(15),
	m_configFont(13)
{

	::ClearRateInit(m_configs[ClearRate]);
	::CircleCutInit(m_configs[CircleCut]);
	::PlayScaleInit(m_configs[PlayScale]);

}

ConfigScene::~ConfigScene()
{}


//--------------------------------------------------------------------------------
//関数：update
//--------------------------------------------------------------------------------


void ConfigScene::update()
{
	m_timer = 19200 * Sin(System::FrameCount() / 200.0);

	if (m_keyConfigEasing.isActive())
	{
		return;
	}

	if (m_isKeyConfig)
	{
		if (!m_keyConfig.update())
		{
			m_isKeyConfig = false;
			m_keyConfigEasing.start();
		}
	}
	else if (m_isSeConfig)
	{
		if (!m_seConfig.update())
		{
			m_isSeConfig = false;
		}
	}
	else
	{
		if (m_mode != Mode::ClearRate)
		{
			if (PlayKey::Up().clicked)
			{
				SoundAsset(L"select").playMulti(0.5);
				m_mode = static_cast<Mode>(static_cast<unsigned>(m_mode) - 1);
			}
		}
		if (m_mode != KeyConfig)
		{
			if (PlayKey::Down().clicked)
			{
				SoundAsset(L"select").playMulti(0.5);
				m_mode = static_cast<Mode>(static_cast<unsigned>(m_mode) + 1);
			}
		}
		switch (m_mode)
		{
		case Mode::ClearRate:
		case Mode::CircleCut:
		case Mode::PlayScale:m_configs.at(m_mode).update();
			break;
		case Mode::TapSE:
			if (PlayKey::Start().clicked)
			{
				SoundAsset(L"select").playMulti(0.5);
				m_isSeConfig = true;
			}
			break;
		case Mode::KeyConfig:
			if (PlayKey::Start().clicked)
			{
				SoundAsset(L"select").playMulti(0.5);
				m_isKeyConfig = true;
				m_keyConfigEasing.start();
			}
			break;
		default:
			break;
		}
	}


	if (PlayKey::BigBack().clicked)
	{
		SoundAsset(L"desisionLarge").playMulti(0.5);
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

	{
		Graphics2D::SetTransform(Mat3x2::Translate(0, m_keyConfigEasing.easeInOut() - 600));

		TextureAsset(L"label").draw();

		FontAsset(L"label")(L"CONFIG").draw(10, 33);

		if (m_isSeConfig)
		{
			m_seConfig.draw(m_font, m_configFont);
		}
		else
		{

			const static std::unordered_map<unsigned, String> tag =
			{
				{ 0,L"表示するクリアレート"},
				{ 1,L"円形切り取り" },
				{ 2,L"プレイ画面の拡大率" },
				{ 3,L"タップ音" },
				{ 4,L"キーコンフィグ" },
			};
			double offset = 0;

			if (m_mode < TOTAL_CONFIG - 4)
			{
				offset = 110 * m_mode;
			}
			else
			{
				offset = 110 * (TOTAL_CONFIG - 4);
			}
			for (unsigned i = 0; i < TOTAL_CONFIG; ++i)
			{
				const float alpha = i == m_mode ? 1 : 0.5;
				Rect(150, 100 + 110 * i - offset, 500, 90).draw({ ColorF(1,0.6,0.2, alpha),ColorF(0.2, alpha),ColorF(0, alpha),ColorF(0, alpha) });
				m_font(tag.at(i)).draw(160, 110 + 110 * i - offset, ColorF(1, alpha));

				if (i < TapSE)
					m_configs.at(i).draw(m_configFont, 150 + 110 * i - offset);

			}
		}

		Graphics2D::SetTransform(Mat3x2::Identity());
	}

	{
		Graphics2D::SetTransform(Mat3x2::Translate(0, m_keyConfigEasing.easeInOut()));

		m_keyConfig.draw();

		Graphics2D::SetTransform(Mat3x2::Identity());
	}

	SceneInfo::Draw(L"Enter:決定 BackSpace 戻る Esc:タイトルに戻る");
}

//--------------------------------------------------------------------------------
//関数：drawFadeIn
//--------------------------------------------------------------------------------
void ConfigScene::drawFadeIn(double t) const
{

	draw();
	FadeIn(t, Fade::SmoothCircle);

}

//--------------------------------------------------------------------------------
//関数：drawFadeOut
//--------------------------------------------------------------------------------
void ConfigScene::drawFadeOut(double t) const
{

	draw();
	FadeOut(t, Fade::SmoothCircle);

}

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
			auto name = FileSystem::Relative(path,L"TapSE");
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
		else if (configParm ==defaultSEPath)
			config.init(L"デフォルト");
		else
			config.init(FileSystem::Relative(configParm, L"TapSE"));
	}
}

TapSEConfig::TapSEConfig() :
	m_mode(Mode::Perfect)
{

	::InitTapSE(m_configs[Perfect], Game::Instance()->m_config.m_perfectSE, L"PERFECT", L"Resource/Sound/SE/tapP.wav");
	::InitTapSE(m_configs[Great], Game::Instance()->m_config.m_greatSE, L"GREAT", L"Resource/Sound/SE/tapGR.wav");
	::InitTapSE(m_configs[Good], Game::Instance()->m_config.m_goodSE, L"GOOD", L"Resource/Sound/SE/tapGD.wav");

}

//-------------------------
bool TapSEConfig::update()
{

	if (PlayKey::SmallBack().clicked)
	{
		SoundAsset(L"cancel").playMulti(0.5);
		return false;
	}
	if (m_mode != Mode::Perfect)
	{
		if (PlayKey::Up().clicked)
		{
			SoundAsset(L"select").playMulti(0.5);
			m_mode = static_cast<Mode>(static_cast<unsigned>(m_mode) - 1);
		}
	}
	if (m_mode != Mode::Good)
	{
		if (PlayKey::Down().clicked)
		{
			SoundAsset(L"select").playMulti(0.5);
			m_mode = static_cast<Mode>(static_cast<unsigned>(m_mode) + 1);
		}
	}
	m_configs.at(m_mode).update();




	return true;
}

void TapSEConfig::draw(const Font & font, const Font & configFont) const
{
	const static std::unordered_map<unsigned, String> tag =
	{
		{ 0,L"PERFECT" },
		{ 1,L"GREAD" },
		{ 2,L"GOOD" },
	};

	for (unsigned i = 0; i < TOTAL_CONFIG; ++i)
	{
		const float alpha = i == m_mode ? 1 : 0.5;
		Rect(150, 100 + 110 * i, 500, 90).draw({ ColorF(1,0.6,0.2, alpha),ColorF(0.2, alpha),ColorF(0, alpha),ColorF(0, alpha) });
		font(tag.at(i)).draw(160, 110 + 110 * i, ColorF(1, alpha));

		m_configs.at(i).draw(configFont, 150 + 110 * i);

	}
}
