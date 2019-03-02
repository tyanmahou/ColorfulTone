#include "SharedDraw.hpp"
#include "BGLight.hpp"

namespace SharedDraw
{
	void DrawBGLight::update()
	{
		if (System::FrameCount() % 20 == 0)
		{
			m_effect.add<BGLight>();
		}
	}

	void DrawBGLight::draw() const
	{
		m_effect.update();
	}

	void Sticky(const String * blueText, const String * redText)
	{
		const auto& font = FontAsset(L"bpm");
		// red
		{
			util::Transformer2D t2d(Mat3x2::Rotate(Math::Radians(-15)).translate({ 110, 50 }));
			TextureAsset(L"sticky_red").draw();
			if (redText)
			{
				util::ContractionDrawbleString(
					font(*redText),
					{ 125,25 },
					175,
					Palette::Black
				);
			}
		}
		// blue
		{
			util::Transformer2D t2d(Mat3x2::Rotate(Math::Radians(-20)).translate({ -25, 60 }));
			TextureAsset(L"sticky").draw();
			if (blueText)
			{
				util::ContractionDrawbleString(
					font(*blueText),
					{ 125,25 },
					175,
					Palette::Black
				);
			}
		}
	}

	JacketInfo::JacketInfo() :
		m_font12(FontAsset(L"bpm")),
		m_font16b(FontAsset(L"selectMusics")),
		m_pos(Constants::JacketCenter, 475)
	{}

	JacketInfo & JacketInfo::setPos(const Vec2 & pos)
	{
		m_pos = pos;
		return *this;
	}

	const JacketInfo& JacketInfo::drawLine() const
	{
		TextureAsset(L"line").drawAt(m_pos);
		return *this;
	}

	const JacketInfo& JacketInfo::drawTitle(const String & title, const Color& color) const
	{
		util::ContractionDrawbleString(
			m_font16b(title),
			m_pos + Vec2{ 0,-35 },
			Constants::JacketWidth,
			color
		);
		return *this;
	}

	const JacketInfo & JacketInfo::drawSub(const String & sub, const Color& color) const
	{
		util::ContractionDrawbleString(
			m_font12(sub),
			m_pos,
			Constants::JacketWidth,
			color
		);
		return *this;
	}

	const JacketInfo & JacketInfo::drawDetail(const String & detail, const Color& color) const
	{
		util::ContractionDrawbleString(
			m_font12(detail),
			m_pos + Vec2{0, 35},
			Constants::JacketWidth,
			color
		);
		return *this;
	}
	const JacketInfo & JacketInfo::drawDetailRight(const String & detail, const Color & color) const
	{
		const int wSize = 12;
		const int width = wSize * detail.length;

		const auto kinetic = [wSize](KineticTypography& k)
		{
			k.pos.x = k.origin.x + wSize * k.index;
		};
		m_font12(detail).drawKinetic(
			m_pos + Vec2{ Constants::JacketWidth / 2.0 - width, 20 },
			kinetic,
			color
		);
		return *this;
	}
}