#pragma once

#include<Siv3D.hpp>
class AudioSpectrum
{
private:
	double m_radius;
	double m_baseHz;
public:
	AudioSpectrum(double radius=180,float baseHz=60):
		m_radius(radius),
		m_baseHz(baseHz)
	{}
	void draw(const Sound& sound)const;
};