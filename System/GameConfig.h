#pragma once

#include<Siv3D.hpp>
#include"PlayBG.h"
#include"PlayStyle.h"
#include"TapSE.hpp"
enum class IndicateRate
{
	Up = 0,
	Down = 1,
	Life = 2,
};
//コンフィグ設定変数
class GameConfig
{
private:

public:
	float m_scrollRate;
	 Key m_red1, m_red2,m_red3;
	 Key m_blue1, m_blue2, m_blue3;
	 Key m_yellow1, m_yellow2,m_yellow3;
	 Key m_up, m_down, m_right, m_left;
	 Key m_start, m_smallBack, m_bigBack;

	 //円切り取り
	 bool m_isCirleCut = true;
	 //表示レート
	 IndicateRate m_rateType = IndicateRate::Up;
	 float m_playScale=1.0;
	 // タイミング調整
	 int8 m_timingAdjust = 0;

	 //タップ音
	 TapSE m_tapSE;

	 //BGM SE音量
	 float m_bgmVolume, m_seVolume, m_masterVolume;

	 //背景演出
	 BGType m_bgType=BGType::Default;
	 bool m_isSpectrum=true;
	 PlayStyleType m_styleType = PlayStyleType::Default;


	 void init();
	 void save();
	 GameConfig();

};