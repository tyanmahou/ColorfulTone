#include <scenes/Scene/Config/ConfigMain.hpp>
#include <Useful.hpp>
#include <scenes/Scene/Config/KeyConfigManager.hpp>

namespace ct
{

    namespace
    {
        void InitTapSE(Config& config)
        {
            for (const auto& se : Game::TapSEs()) {
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
            double m_volume = 0.0;
        public:
            TapSEConfig()
            {
                // BGM音量を下げる
                m_volume = SoundManager::GetBgmVolume();
                SoundManager::FadeBgmVolume(s3d::Min(0.1, m_volume), 0.5s);

                m_configs.resize(TOTAL_CONFIG);
                m_configs[All].setName(U"タップ音");
                InitTapSE(m_configs[All]);
                m_configs[Perfect].setName(U"PERFECT 試聴");
                m_configs[Perfect].applyOnEnterd([]() {
                    SoundManager::PlayInGameSe(U"PERFECT");
                    });

                m_configs[Great].setName(U"GREAT 試聴");
                m_configs[Great].applyOnEnterd([]() {
                    SoundManager::PlayInGameSe(U"GREAT");
                    });

                m_configs[Good].setName(U"GOOD 試聴");
                m_configs[Good].applyOnEnterd([]() {
                    SoundManager::PlayInGameSe(U"GOOD");
                    });
            }
            ~TapSEConfig()
            {
                // BGM音量を戻す
                SoundManager::FadeBgmVolume(m_volume, 0.5s);
            }
        };
    }

    namespace
    {
        //volume
        void VolumeInit(Config& config, std::function<void(double)> func, double& configParam, const String& defaultValue = U"x1.0")
        {
            static const std::map<double, String> map
            {
                { 0.0 ,U"x0.0" },
                { 0.1 ,U"x0.1" },
                { 0.2 ,U"x0.2" },
                { 0.3 ,U"x0.3" },
                { 0.4 ,U"x0.4" },
                { 0.5 ,U"x0.5" },
                { 0.6, U"x0.6" },
                { 0.7, U"x0.7" },
                { 0.8, U"x0.8" },
                { 0.9, U"x0.9" },
                { 1.0, U"x1.0" },
            };
            for (auto&& pair : map) {
                config.add(pair.second, [&pair, func, &configParam]() {
                    configParam = pair.first;
                    func(pair.first);
                    });
            }
            config.setDefault(defaultValue);
            if (map.find(configParam) != map.end()) {
                config.init(map.at(configParam));
            } else {
                config.init(defaultValue);
            }
        }
        class VolumeConfig :public IConfigHierchy
        {
            enum
            {
                BGMVolume,
                SEVolume,
                InGameMusicVolume,
                InGameSeVolume,
                MasterVolume,
                TOTAL_SIZE
            };
        public:
            VolumeConfig()
            {
                m_configs.resize(TOTAL_SIZE);
                m_configs[BGMVolume].setName(U"BGM");
                VolumeInit(m_configs[BGMVolume], SoundManager::SetBgmVolume,
                    Game::Config().m_bgmVolume);

                m_configs[SEVolume].setName(U"SE");
                VolumeInit(m_configs[SEVolume], SoundManager::SetSeVolume,
                    Game::Config().m_seVolume);

                m_configs[InGameMusicVolume].setName(U"プレイ中 楽曲");
                VolumeInit(m_configs[InGameMusicVolume], SoundManager::SetInGameMusicVolume,
                    Game::Config().m_inGameMusicVolume);

                m_configs[InGameSeVolume].setName(U"プレイ中 SE");
                VolumeInit(m_configs[InGameSeVolume], SoundManager::SetInGameSeVolume,
                    Game::Config().m_inGameSeVolume);

                m_configs[MasterVolume].setName(U"Master");
                VolumeInit(m_configs[MasterVolume], SoundManager::SetMasterVolume,
                    Game::Config().m_masterVolume);
            }
        };
    }

    namespace
    {
        //表示クリアレート
        void ClearRateInit(Config& config)
        {
            config.setName(U"表示するクリアレート");
            config.add(U"加算式", []() {Game::Config().m_rateType = IndicateRate::Up; });
            config.add(U"減算式", []() {Game::Config().m_rateType = IndicateRate::Down; });
            config.add(U"ライフゲージ", []() {Game::Config().m_rateType = IndicateRate::Life; });

            if (Game::Config().m_rateType == IndicateRate::Up) {
                config.init(U"加算式");
            } else if (Game::Config().m_rateType == IndicateRate::Down) {
                config.init(U"減算式");
            } else if (Game::Config().m_rateType == IndicateRate::Life) {
                config.init(U"ライフゲージ");
            }
        }
        //円形切り取りの初期化
        void CircleCutInit(Config& config)
        {
            config.setName(U"円形切り取り");
            config.add(U"ON", []() {Game::Config().m_isCirleCut = true; });
            config.add(U"OFF", []() {Game::Config().m_isCirleCut = false; });

            if (Game::Config().m_isCirleCut)
                config.init(U"ON");
            else
                config.init(U"OFF");
        }
        //プレイスケール
        void PlayScaleInit(Config& config)
        {
            config.setName(U"プレイ画面の拡大率");
            static const std::map<double, String> map
            {
                { 0.5,U"x0.5" },
                { 0.6, U"x0.6" },
                { 0.7, U"x0.7" },
                { 0.8, U"x0.8" },
                { 0.9, U"x0.9" },
                { 1.0, U"x1.0" },
                { 1.1, U"x1.1" },
                { 1.2, U"x1.2" },
            };
            for (auto&& pair : map) {
                config.add(pair.second, [&pair]() {Game::Config().m_playScale = pair.first; });
            }
            config.setDefault(U"x1.0");
            config.init(map.at(Game::Config().m_playScale));
        }
        void TimingAdjustInit(Config& config)
        {
            config.setName(U"タイミング調整");
            for (int32 adjust : step_to(-10, 10)) {
                config.add(Format(adjust), [=]() {Game::Config().m_timingAdjust = static_cast<int8>(adjust); });
            }
            config.setDefault(U"0");
            config.init(Format(Game::Config().m_timingAdjust));
        }
        void PlayBGInit(Config& config)
        {
            config.setName(U"背景設定");

            config.add(U"デフォルト", []() {Game::Config().m_bgType = BGType::Default; });
            config.add(U"ガウスぼかし", []() {Game::Config().m_bgType = BGType::Blur; });
            config.add(U"黒", []() {Game::Config().m_bgType = BGType::Black; });
            config.add(U"白", []() {Game::Config().m_bgType = BGType::White; });
            config.add(U"背景なし 黒", []() {Game::Config().m_bgType = BGType::NoneBlack; });
            config.add(U"背景なし 白", []() {Game::Config().m_bgType = BGType::NoneWhite; });

            config.init(static_cast<size_t>(Game::Config().m_bgType));
        }
        void PlayBGBrightnessInit(Config& config)
        {
            config.setName(U"背景明るさ");
            for (int32 adjust : step_to(0, 10)) {
                config.add(Format(adjust), [=]() {Game::Config().m_bgBrightness = static_cast<int8>(adjust); });
            }
            config.setDefault(U"10");
            config.init(Format(Game::Config().m_bgBrightness));
        }
        //style
        void PlayStyleInit(Config& config)
        {
            config.setName(U"プレイモード");

            config.add(U"通常モード", [] {
                Game::Config().m_styleType = PlayStyleType::Default;
                });
            config.add(U"アークモード", [] {
                Game::Config().m_styleType = PlayStyleType::NormalArc;
                });
            config.add(U"縦レーン", [] {
                Game::Config().m_styleType = PlayStyleType::Portrait;
                });

            switch (Game::Config().m_styleType) {
            case PlayStyleType::Default: config.init(U"通常モード");
                break;
            case PlayStyleType::NormalArc: config.init(U"アークモード");
                break;
            case PlayStyleType::Portrait: config.init(U"縦レーン");
                break;
            }
        }
        //オーディオスペクトラムの初期化
        void IsSpectrumInit(Config& config)
        {
            config.setName(U"オーディオスペトラムの表示");
            config.add(U"ON", []() {Game::Config().m_isSpectrum = true; });
            config.add(U"OFF", []() {Game::Config().m_isSpectrum = false; });

            if (Game::Config().m_isSpectrum)
                config.init(U"ON");
            else
                config.init(U"OFF");
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
                BGBrightness,
                IsSpectrum,
                Style,
                TOTAL_CONFIG //コンフィグの数
            };
        public:
            PlayConfig()
            {
                m_configs.resize(TOTAL_CONFIG);
                ClearRateInit(m_configs[ClearRate]);
                CircleCutInit(m_configs[CircleCut]);
                PlayScaleInit(m_configs[PlayScale]);
                TimingAdjustInit(m_configs[TimingAdjust]);
                PlayBGInit(m_configs[BGType]);
                PlayBGBrightnessInit(m_configs[BGBrightness]);
                IsSpectrumInit(m_configs[IsSpectrum]);
                PlayStyleInit(m_configs[Style]);
            }
        };

    }
    namespace
    {


        class MainConfig :public IConfigHierchy
        {

            KeyConfigManager m_keyConfig;

            EasingAB<double> m_keyConfigEasing;

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
                m_configs[Play].setName(U"プレイ画面");
                m_configs[Play].applyOnEnterd([this]() {
                    this->changePush<PlayConfig>();
                    });
                m_configs[Volume].setName(U"音量");
                m_configs[Volume].applyOnEnterd([this]() {
                    this->changePush<VolumeConfig>();
                    });

                m_configs[TapSE].setName(U"タップ音");
                m_configs[TapSE].applyOnEnterd([this]() {
                    this->changePush<TapSEConfig>();
                    });
                m_configs[KeyConfig].setName(U"キーコンフィグ");
                m_configs[KeyConfig].applyOnEnterd([this]() {
                    m_isKeyConfig = true;
                    m_keyConfigEasing.start();
                    });
            }

            bool update()override
            {
                if (m_keyConfigEasing.isMoving()) {
                    return true;
                }

                if (m_isKeyConfig) {
                    if (!m_keyConfig.update()) {
                        m_isKeyConfig = false;
                        m_keyConfigEasing.start();
                    }
                } else {
                    return IConfigHierchy::update();
                }
                return true;
            }

            void draw()const override
            {
                {
                    Transformer2D t2d(Mat3x2::Translate(0, m_keyConfigEasing.easeInOut() - 600));

                    IConfigHierchy::draw();
                }
                {
                    Transformer2D t2d(Mat3x2::Translate(0, m_keyConfigEasing.easeInOut()));

                    m_keyConfig.draw();
                }
            }
        };
    }

    ConfigMain::ConfigMain()
    {
        m_config.changePush<MainConfig>();
    }
    bool ConfigMain::update()
    {
        if (!m_isActive) {
            return false;
        }
        return m_config.update();
    }

    void ConfigMain::draw() const
    {
        if (!m_isActive) {
            return;
        }
        m_config.draw();
    }

    void ConfigMain::reset()
    {
        m_config.clear();
        m_config.changePush<MainConfig>();
    }

    bool ConfigMain::isRoot() const
    {
        return m_config.isRoot();
    }

}