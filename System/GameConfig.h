#pragma once

#include<Siv3D.hpp>
#include"PlayBG.h"
//�R���t�B�O�ݒ�ϐ�
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

	 //�~�؂���
	 bool m_isCirleCut = false;
	 //���Z���ɂ��邩
	 bool m_isClearRateDownType = false;
	 float m_playScale=1.0;

	 //�^�b�v��
	 String m_perfectSE, m_greatSE, m_goodSE;

	 //BGM SE����
	 float m_bgmVolume, m_seVolume;

	 //�w�i���o
	 BGType m_bgType=BGType::Default;
	 bool m_isSpectrum=true;

	 void init();
	 void save();
	 GameConfig();

};