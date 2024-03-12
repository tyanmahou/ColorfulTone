#include <scenes/utils/SharedDraw/JacketInfo.hpp>
#include <commons/Constants.hpp>
#include <commons/FontName.hpp>
#include <scenes/utils/Util.hpp>
#include <Siv3D.hpp>

namespace ct::SharedDraw
{
	JacketInfo::JacketInfo() :
		m_font12(FontAsset(FontName::Bpm)),
		m_font16b(FontAsset(FontName::SelectMusic)),
		m_pos(Constants::JacketCenter, 475)
	{}

	JacketInfo& JacketInfo::setPos(const Vec2& pos)
	{
		m_pos = pos;
		return *this;
	}

	const JacketInfo& JacketInfo::drawLabel(const Texture& tex, double t)const
	{
		tex.uv(0, 100.0 / 800.0, t, 30.0 / 800.0).resized(800 * t, 30).draw();
		tex.uv(1 - t, 630.0 / 800.0, t, 30.0 / 800.0).resized(800 * t, 30).draw(800 - 800 * t, 530);
		return *this;
	}

	const JacketInfo& JacketInfo::drawLabel()const
	{
		Rect(0, 0, 800, 30).draw(Color(0, 0, 0, 150));
		Rect(0, 530, 800, 30).draw(Color(0, 0, 0, 150));
		return *this;
	}

	const JacketInfo& JacketInfo::drawLine() const
	{
		TextureAsset(U"line").drawAt(m_pos);
		return *this;
	}

	const JacketInfo& JacketInfo::drawTitle(const String& title, const Color& color) const
	{
		ContractionDrawbleString(
			m_font16b(title),
			m_pos + Vec2{ 0,-35 },
			Constants::JacketWidth,
			color
		);
		return *this;
	}

	const JacketInfo& JacketInfo::drawSub(const String& sub, const Color& color) const
	{
		ContractionDrawbleString(
			m_font12(sub),
			m_pos,
			Constants::JacketWidth,
			color
		);
		return *this;
	}

	const JacketInfo& JacketInfo::drawDetail(const String& detail, const Color& color) const
	{
		ContractionDrawbleString(
			m_font12(detail),
			m_pos + Vec2{ 0, 35 },
			Constants::JacketWidth,
			color
		);
		return *this;
	}
	const JacketInfo& JacketInfo::drawDetailRight(const String& detail, const Color& color) const
	{
		const int32 wSize = 12;
		const int32 width = static_cast<int32>(wSize * detail.length());

		const Vec2 topLeft = m_pos + Vec2{ Constants::JacketWidth / 2.0 - width, 20 };
		for (const auto& [i, glyph] : s3d::Indexed(m_font12.getGlyphs(detail))) {
			Vec2 drawPos = topLeft + glyph.getOffset();
			drawPos.x += wSize * i;
			glyph.texture.draw(Math::Round(drawPos), color);
		}
		return *this;
	}
}
