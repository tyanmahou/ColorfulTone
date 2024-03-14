#include <core/Play/Effects/TapEffect.hpp>
#include <Siv3D.hpp>
namespace ct
{
	using s3d::Math::Pi;
	TapEffect::TapEffect(const s3d::Vec2& pos, const double angle, s3d::int32 type) :
		m_pos(pos),
		m_angle(angle + Pi),
		m_type(type),
		m_sImage(TextureAsset(U"tapEffct"), { 20,1 }, true)
	{}
	TapEffect::TapEffect(const double angle, s3d::int32 type) :
		TapEffect({400, 300}, angle, type)
	{}
	bool TapEffect::update(double t)
	{
		if (m_type != 9) {
			TextureAsset(Format(U"center_", m_type)).drawAt(m_pos);
		}
		m_sImage.update();
		m_sImage.rotate(m_angle).drawAt(m_pos);
		const int32 alpha = Min(static_cast<int32>(- t * 10000.0 / 34.0 + 110), 255);
		Color color = Palette::Blue;
		color.setA(alpha);
		Circle(m_pos, 40 + 70 * t).drawFrame(1, 0, color);
		return t <= 0.34;
	}
	TapEffect2::TapEffect2(double posX, double width) :
		m_posX(posX),
		m_width(width)
	{}
	bool TapEffect2::update(double t)
	{
		double y = -100 * t + 500;
		double x = m_posX - m_width / 2;
		const int32 alpha = Min(static_cast<int32>(-t * 10000.0 / 34.0 + 110), 255);
		Color color = Palette::Blue;
		color.setA(alpha);
		Line({ x, y }, { x + m_width,y }).draw(2, color);
		return t <= 0.34;
	}
	TapEffect2_2::TapEffect2_2(double posX) :
		m_posX(posX)
	{}
	TapEffect2_2::TapEffect2_2(const Vec2 & pos, double scale) :
		m_posX(pos.x),
		m_posY(pos.y),
		m_scale(scale)
	{}
	bool TapEffect2_2::update(double t)
	{
		t *= 5.0;
		TextureAsset(U"tapEffect2").scaled(m_scale * 1.2 * EaseOutElastic(t)).drawAt(m_posX, m_posY, AlphaF(1 - t));
		return t <= 1.0;
	}
}