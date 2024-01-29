#pragma once

#include <Siv3D/Input.hpp>
#include <core/Play/PlayBG/BGType.hpp>
#include <core/Play/PlayStyle/PlayStyleType.hpp>
#include <core/Data/TapSE/TapSE.hpp>

namespace ct
{
    enum class IndicateRate
    {
        Up = 0,
        Down = 1,
        Life = 2,
    };
    //コンフィグ設定変数
    class GameConfig
    {
    public:
        GameConfig();

        void init();
        void save();
    public:
        double m_scrollRate;
        s3d::Input m_red1, m_red2, m_red3;
        s3d::Input m_blue1, m_blue2, m_blue3;
        s3d::Input m_yellow1, m_yellow2, m_yellow3;
        s3d::Input m_up, m_down, m_right, m_left;
        s3d::Input m_start, m_smallBack, m_bigBack;

        //円切り取り
        bool m_isCirleCut = true;
        //表示レート
        IndicateRate m_rateType = IndicateRate::Up;
        double m_playScale = 1.0;
        // タイミング調整
        s3d::int8 m_timingAdjust = 0;
        bool m_isLifeDead = false;

        //タップ音
        TapSE m_tapSE;

        //BGM SE音量
        double m_bgmVolume, m_seVolume, m_inGameMusicVolume, m_inGameSeVolume, m_masterVolume;

        //背景演出
        BGType m_bgType = BGType::Default;
        bool m_isSpectrum = true;
        PlayStyleType m_styleType = PlayStyleType::Default;
        s3d::int8 m_bgBrightness = 10;
    };
}