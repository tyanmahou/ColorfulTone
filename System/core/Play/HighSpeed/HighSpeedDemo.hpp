#pragma once
#include <memory>
#include <Siv3D/Array.hpp>
#include <Siv3D/EasingAB.hpp>
#include <Siv3D/Rect.hpp>
#include <core/Play/PlayStyle/PlayStyleType.hpp>

namespace ct
{
	class Object;
	class SoundBar;

	class HighSpeedDemo
	{
	public:
		HighSpeedDemo();
		~HighSpeedDemo();
		bool update(double& scrollRate);
		void draw(const SoundBar& min, const SoundBar& max, double scrollRate)const;
	private:
		void resetStyle(PlayStyleType style);
		void drawDemoNotes(const s3d::Rect& rect, const SoundBar& bar, double scrollRate, size_t index) const;
	private:
		std::shared_ptr<Object> m_bar;
		std::shared_ptr<Object> m_note;

		s3d::EasingAB<s3d::int32> m_offset;
		const s3d::Rect m_bgRect;

		PlayStyleType m_style;
		s3d::RenderTexture m_renderTexture[2];
	};
}
