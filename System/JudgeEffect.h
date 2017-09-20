#pragma once
#include"Game.h"

struct JudgeEffect : IEffect
{
	const Vec2 m_from;

	const String m_name;

	JudgeEffect(String name, const Vec2& from):
		m_from(from),
		m_name(name){}

	bool update(double t) override
	{
		if (t > 0.5)
		{
			return false;
		}

		t *= (1 / 0.5);

		const double alpha = 1.0 - t;

		const double offset = -20 * t;

		FontAsset(L"judge")(m_name).drawCenter(m_from + Vec2(0, offset), ColorF(0, alpha));

		return true;
	}
};