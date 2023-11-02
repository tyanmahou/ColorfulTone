#pragma once
#include <Siv3D/Effect.hpp>
#include <utils/TimeLite/IntervalTimer.hpp>

namespace ct::SharedDraw
{
	class DrawBGLight
	{
	public:
		DrawBGLight();
		void update();
		void draw()const;
	private:
		s3d::Effect m_effect;
		TimeLite::IntervalTimer m_timer;
	};
}