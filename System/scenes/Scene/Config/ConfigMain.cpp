#include <scenes/Scene/Config/ConfigMain.hpp>
#include <Useful.hpp>
#include <scenes/Scene/Config/KeyConfigManager.hpp>
#include <scenes/Scene/ISceneBase.hpp>
#include <core/Play/LifeGauge/LifeGauge.hpp>

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
            }
            config.init(Game::Config().m_tapSE.getName());
        }
        class TapSEConfig :public IConfigHierchy
        {
            enum Mode :s3d::int32
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
                m_configs[Perfect].setNeedEnterdSe(false);
                m_configs[Great].setName(U"GREAT 試聴");
                m_configs[Great].applyOnEnterd([]() {
                    SoundManager::PlayInGameSe(U"GREAT");
                    });
                m_configs[Great].setNeedEnterdSe(false);

                m_configs[Good].setName(U"GOOD 試聴");
                m_configs[Good].applyOnEnterd([]() {
                    SoundManager::PlayInGameSe(U"GOOD");
                    });
                m_configs[Good].setNeedEnterdSe(false);
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
        // 表示設定
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

        //背景エフェクトの初期化
        void UseBgEffect(Config& config)
        {
            config.setName(U"背景エフェクト");
            config.add(U"ON", []() {Game::Config().m_useBgEffect = true; });
            config.add(U"OFF", []() {Game::Config().m_useBgEffect = false; });

            if (Game::Config().m_useBgEffect)
                config.init(U"ON");
            else
                config.init(U"OFF");
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
        class PlayViewConfig :public IConfigHierchy
        {
            enum Mode
            {
                BGType,
                BGBrightness,
                BGEffect,
                IsSpectrum,
                CircleCut,
                TOTAL_CONFIG //コンフィグの数
            };
        public:
            PlayViewConfig()
            {
                m_configs.resize(TOTAL_CONFIG);
                PlayBGInit(m_configs[BGType]);
                PlayBGBrightnessInit(m_configs[BGBrightness]);
                UseBgEffect(m_configs[BGEffect]);
                IsSpectrumInit(m_configs[IsSpectrum]);
                CircleCutInit(m_configs[CircleCut]);
            }
        };
    }
    namespace
    {
        //style
        void PlayStyleInit(Config& config)
        {
            config.setName(U"プレイモード");

            static Array<std::pair<StringView, PlayStyleType>> list
            {
                {U"通常モード", PlayStyleType::Default},
                {U"アークモード", PlayStyleType::NormalArc},
                {U"縦レーン", PlayStyleType::Portrait},
                {U"奥レーン", PlayStyleType::Homography},
                {U"横レーン", PlayStyleType::Landscape},
            };
            for (auto&& [title, style] : list) {
                String str{ title };
                auto event = [style] {
                    Game::Config().m_styleType = style;
                    };
                config.add(str, std::move(event));

                if (Game::Config().m_styleType == style) {
                    config.init(str);
                }
            }
            config.setExtention([&](size_t index, double y) {
                TextureAsset(U"playstyle_icon")(static_cast<int32>(list[index].second) * 50, 0, 50, 50).drawAt(500 - 85, y, Palette::Orange);
                });
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

        // ランダム
        void RandomInit(Config& config)
        {
            config.setName(U"配置設定");
            static std::array<std::pair<RandomNoteType, StringView>, 9> list
            {
                std::pair{RandomNoteType::None, U""},
                std::pair{RandomNoteType::Mirror, U"赤と黄の配置を反転します"},
                std::pair{RandomNoteType::Rotate120, U"赤を黄, 青を赤, 黄を青に変更します"},
                std::pair{RandomNoteType::Rotatee120Mirror, U"青と黄の配置を反転します"},
                std::pair{RandomNoteType::Rotate240, U"赤を青, 青を黄, 黄を赤に変更します"},
                std::pair{RandomNoteType::Rotate240Mirror, U"赤と青の配置を反転します"},
                std::pair{RandomNoteType::Random, U"各色の配置をランダムに変更します"},
                std::pair{RandomNoteType::BarRandom, U"1小節ごとに配置をランダムに変更します"},
                std::pair{RandomNoteType::SRandom, U"ノーツごとに配置をランダムに変更します"}
            };
            for (auto [type, detail] : list) {
                config.add({ ToRandomNoteTypeStr(type) }, [type]() {Game::Config().m_random = type; }, { detail });
            }
            config.init(static_cast<size_t>(Game::Config().m_random));
        }
        void LifeGaugeInit(Config& config)
        {
            config.setName(U"ライフゲージ");

            static Array<std::pair<StringView, LifeGaugeKind>> list
            {
                {U"INVINCIBLE", LifeGaugeKind::Invincible},
                {U"SAFETY", LifeGaugeKind::Safety},
                {U"SUPPORT", LifeGaugeKind::Support},
                {U"NORMAL", LifeGaugeKind::Normal},
                {U"CHALLENGE", LifeGaugeKind::Challenge},
                {U"DANGER"     , LifeGaugeKind::Danger},
                {U"SUDDENDEATH", LifeGaugeKind::SuddenDeath},
            };
            for (auto&& [title, kind] : list) {
                String str{ title };
                auto event = [kind] {
                    Game::Config().m_lifeGauge = kind;
                    };
                auto gauge = LifeRecoverySet::FromKind(kind);
                String detail = U"[PERFECT] {:+.2f}%, [GREAT] {:+.2f}%, [GOOD] {:+.2f}%, [MISS] {:+.2f}%"_fmt(
                    static_cast<double>(gauge.perfect) / 100.0,
                    static_cast<double>(gauge.great) / 100.0,
                    static_cast<double>(gauge.good) / 100.0,
                    static_cast<double>(gauge.miss) / 100.0
                );
                if (kind == LifeGaugeKind::Invincible) {
                    detail = U"変動なし(※コース合格になりません)";
                }
                config.add(str, std::move(event), detail);
                if (Game::Config().m_lifeGauge == kind) {
                    config.init(str);
                }
            }
            config.setExtention([&](size_t index, double y) {
                ColorF color = config.isActive() ? ColorF(Palette::White) : ColorF(0.5, 1);
                LifeGauge::GetBadge(list[index].second).drawAt(500 - 90, y, color);
           });
        }
        void LifeDeadInit(Config& config)
        {
            config.setName(U"FREE PLAY ライフモード");
            config.add(U"OFF", []() {Game::Config().m_isLifeDead = false; });
            config.add(U"ON", []() {Game::Config().m_isLifeDead = true; }, U"FREE PLAY中もライフが0になると強制終了となります");

            if (Game::Config().m_isLifeDead)
                config.init(U"ON");
            else
                config.init(U"OFF");

        }
        void TimingAdjustInit(Config& config)
        {
            config.setName(U"タイミング調整");
            for (int32 adjust : step_to(-10, 10)) {
                config.add(Format(adjust), [=]() {Game::Config().m_timingAdjust = static_cast<int8>(adjust); }, U"判定のタイミングを調整します。(ノーツがラインと重なるタイミングは変りません。)");
            }
            config.setDefault(U"0");
            config.init(Format(Game::Config().m_timingAdjust));
        }
        void OffsetAdjustInit(Config& config)
        {
            config.setName(U"オフセット調整");
            for (int32 adjust : step_to(-10, 10)) {
                config.add(Format(adjust), [=]() {Game::Config().m_offsetAdjust = static_cast<int8>(adjust); }, U"ノーツがラインと重なるタイミングを調整します。(判定のタイミングは変りません。)");
            }
            config.setDefault(U"0");
            config.init(Format(Game::Config().m_offsetAdjust));
        }
        // 判定アルゴリズム
        void JudgeAlgoInit(Config& config)
        {
            config.setName(U"判定アルゴリズム設定");
            config.add(U"Earliest", []() {Game::Config().m_judgeAlgoKind = JudgeAlgorithmKind::Earliest; }, U"先にきたノーツを優先して判定します");
            config.add(U"Nearest", []() {Game::Config().m_judgeAlgoKind = JudgeAlgorithmKind::Nearest; }, U"PERFECTに近いノーツを優先して判定します");

            config.init(static_cast<size_t>(Game::Config().m_judgeAlgoKind));
        }
        class PlayConfig :public IConfigHierchy
        {
            enum Mode
            {
                Style,
                PlayScale,
                ClearRate,
                ViewConfig,
                Random,
                LifeGauge,
                LifeDead,
                TimingAdjust,
                OffsetAdjust,
                JudgeAlgo,
                TOTAL_CONFIG //コンフィグの数
            };
        public:
            PlayConfig()
            {
                m_configs.resize(TOTAL_CONFIG);
                PlayStyleInit(m_configs[Style]);
                PlayScaleInit(m_configs[PlayScale]);
                ClearRateInit(m_configs[ClearRate]);
                m_configs[ViewConfig].setName(U"背景詳細");
                m_configs[ViewConfig].applyOnEnterd([this]() {
                    this->changePush<PlayViewConfig>();
                    });
                RandomInit(m_configs[Random]);
                LifeGaugeInit(m_configs[LifeGauge]);
                LifeDeadInit(m_configs[LifeDead]);
                TimingAdjustInit(m_configs[TimingAdjust]);
                OffsetAdjustInit(m_configs[OffsetAdjust]);
                JudgeAlgoInit(m_configs[JudgeAlgo]);
            }

            bool update()override
            {
                if (auto* data = this->getGameData()) {
                    m_configs[LifeGauge].setActive(!data->m_course.isActive());
                }
                return IConfigHierchy::update();
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
            double detailAlpha() const override
            {
                return 0.8 * (1 - m_keyConfigEasing.progress0_1());
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

    void ConfigMain::drawWithBack(const s3d::ColorF& color) const
    {
        if (!m_isActive) {
            return;
        }
        Scene::Rect().draw(color);
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