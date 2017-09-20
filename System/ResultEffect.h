#pragma once
#include"Game.h"

struct ResultEffect : IEffect
{
	const Vec2 m_from;

	const String m_name;

	double m_scale;
	ResultEffect(String name, const Vec2& from,double scale=1.0) :
		m_from(from),
		m_name(name),
		m_scale(scale){}

	bool update(double t) override
	{
		if (t > 0.5)
		{
			return false;
		}

		t *= (1 / 0.5);

		const double alpha = 1.0 - t;


		TextureAsset(m_name).scale(m_scale*(1.0+t)).drawAt(m_from, ColorF(0, alpha));

		return true;
	}
};
