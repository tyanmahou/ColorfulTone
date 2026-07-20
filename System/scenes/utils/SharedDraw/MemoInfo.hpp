#pragma once
#include <core/Play/LifeGauge/LifeGaugeKind.hpp>
#include <Siv3D/Vector2D.hpp>

namespace ct
{
	class NotesData;
	class EndlessData;
	struct CourseScore;
	struct EndlessGaugeScore;
	struct EndlessResult;
}
namespace ct::SharedDraw
{
	class MemoInfo
	{
	public:
		static constexpr s3d::Vec2 DefaultPos{ 630, 480 };
	public:
		MemoInfo();
		MemoInfo& setPos(const s3d::Vec2& pos);
		MemoInfo& setPosFromDefault(const s3d::Vec2& pos);
		void draw(const NotesData& notes) const;

		void draw(const CourseScore& courseScore, bool drawFailure = true) const;

		void draw(const EndlessData& endless, LifeGaugeKind gauge) const;
		void draw(const EndlessResult& result) const;
		void draw(const EndlessGaugeScore& score, LifeGaugeKind gauge) const;
	private:
		s3d::Vec2 m_pos;
	};
}