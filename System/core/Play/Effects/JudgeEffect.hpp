#pragma once
#include <Siv3D/IEffect.hpp>
#include <Siv3D/Vector2D.hpp>
#include <Siv3D/String.hpp>

namespace ct
{
	struct JudgeEffect : s3d::IEffect
	{
		JudgeEffect(const s3d::String& name, const s3d::Vec2& from);

		bool update(double t) override;
	private:

		const s3d::Vec2 m_from;
		const s3d::String m_name;
	};
}