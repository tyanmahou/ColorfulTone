#pragma once
#include <Siv3D/Vector2D.hpp>

namespace ct
{
	class NotesData;
	struct CourseScore;
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
	private:
		s3d::Vec2 m_pos;
	};
}