#pragma once
#include <Siv3D/IEffect.hpp>
#include <Siv3D/Vector2D.hpp>
#include <Siv3D/String.hpp>

namespace ct
{
	struct JudgeEffect : s3d::IEffect
	{
		JudgeEffect(Score::Judge judge, s3d::int64 diff, const s3d::Vec2& from);
		JudgeEffect(s3d::StringView name, const s3d::Vec2& from, const s3d::ColorF& color = s3d::Palette::Black);

		bool update(double t) override;
	private:

		const s3d::Vec2 m_from;
		const s3d::StringView m_name;
		s3d::ColorF m_color;
	};
}