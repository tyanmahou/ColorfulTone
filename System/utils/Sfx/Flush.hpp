#pragma once
#include <utils/Sfx/ColorFade.hpp>

namespace ct::Sfx
{
	class Flush
	{
	public:
		void start(double time = 1.0, const s3d::ColorF& color = s3d::Palette::White);
		void start(double fadeInTime, double fadeOutTime, const s3d::ColorF& color = s3d::Palette::White);
		bool isFadeInEnd() const;
		bool isFadeOutEnd() const;

		void reset();

		bool isActive() const;
	public:
		void update(double dt);
		void draw() const;
	private:
		ColorFade m_fade;
	};
}