#pragma once

#include"SerialImage.h"


struct  TapEffect : public IEffect
{
	//エフェクトの中心
	const double m_angle;
	//オフセット
	const int m_type;
	SerialImage m_sImage;
	//コンストラクタ
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

struct  TapEffect2 : public IEffect
{
	double m_posX;
	double m_width;

	TapEffect2(double posX,double width):
		m_posX(posX),
		m_width(width)
	{}

	bool update(double t) override
	{
		double y = -100 * t+500;
		double x = m_posX - m_width / 2;
		const int alpha = -t*10000.0 / 34.0 + 110;
		Line({x, y }, {x+m_width,y}).draw(2, static_cast<Color>(Palette::Blue).setAlpha(alpha));
		return t <= 0.34;
	}
};

struct  TapEffect2_2 : public IEffect
{
	double m_posX;
	double m_posY = 500;
	double m_scale = 1.0;
	TapEffect2_2(double posX) :
		m_posX(posX)
	{}
	TapEffect2_2(const Vec2& pos, double scale = 0.5) :
		m_posX(pos.x),
		m_posY(pos.y),
		m_scale(scale)
	{}

	bool update(double t) override
	{
		t *= 5.0;
		TextureAsset(L"tapEffect2").scale(m_scale * 1.2*EaseOut(Easing::Elastic,t)).drawAt(m_posX, m_posY,AlphaF(1-t));
		return t <= 1.0;
	}
};


