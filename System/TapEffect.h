#pragma once

#include"SerialImage.h"


struct  TapEffect : public IEffect
{
	//�G�t�F�N�g�̒��S
	const double m_angle;
	//�I�t�Z�b�g
	const int m_type;
	SerialImage m_sImage;
	//�R���X�g���N�^
	TapEffect(const double angle,int type=0):
		m_angle(angle+Pi),
		m_type(type),
		m_sImage(TextureAsset(L"tapEffct"),{ 20,1 }, 0,true){}

	bool update(double t) override
	{
		if(m_type!=9)
		TextureAsset(Format(L"center_", m_type)).drawAt(400, 300);
		m_sImage.update();
		m_sImage.rotate(m_angle).drawAt(400, 300);
		const int alpha =  - t*10000.0/34.0+110;
		Circle(400, 300, 40 + 70 * t).drawFrame(1, 0,static_cast<Color>(Palette::Blue).setAlpha(alpha));
		return t <= 0.34;
	}
};

