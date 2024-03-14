#pragma once
#include <utils/Flipbook/Flipbook.hpp>
#include <Siv3D/IEffect.hpp>

namespace ct
{
	struct TapEffect : public s3d::IEffect
	{
		//コンストラクタ
		TapEffect(const s3d::Vec2& pos, const double angle, s3d::int32 type = 0);
		TapEffect(const double angle, s3d::int32 type = 0);

		bool update(double t) override;
	private:
		s3d::Vec2 m_pos;
		//エフェクトの中心
		const double m_angle;
		//オフセット
		const s3d::int32 m_type;
		Flipbook m_sImage;
	};

	struct TapEffect2 : public s3d::IEffect
	{
		TapEffect2(double posX, double width);

		bool update(double t) override;
	private:
		double m_posX;
		double m_width;
	};

	struct TapEffect2_2 : public s3d::IEffect
	{
		TapEffect2_2(double posX);
		TapEffect2_2(const Vec2& pos, double scale = 0.5);

		bool update(double t) override;
	private:
		double m_posX;
		double m_posY = 500;
		double m_scale = 1.0;
	};

}