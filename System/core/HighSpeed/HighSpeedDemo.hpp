#pragma once
#include <Siv3D/Audio.hpp>
#include <Siv3D/Array.hpp>
#include <Siv3D/EasingAB.hpp>
#include <Siv3D/Rect.hpp>
#include <Siv3D/Circle.hpp>
#include <Siv3D/Stopwatch.hpp>
#include <memory>

namespace ct
{
	class Object;
	class SoundBar;

	class HighSpeedDemo
	{
	private:
		s3d::Array<std::shared_ptr<Object>> m_objects;//デモ用譜面データ

		s3d::EasingAB<s3d::int32> m_offset;

		s3d::Audio m_sound;

		const s3d::Rect m_bgRect;
		const s3d::Circle m_judgeCircle;
		s3d::Stopwatch m_stopwatch;
		void drawDemoNotes(const SoundBar& bar, double scrollRate) const;
	public:
		HighSpeedDemo();
		~HighSpeedDemo();
		bool update(double& scrollRate);
		void draw(const SoundBar& min, const SoundBar& max, double scrollRate)const;
	};
}
