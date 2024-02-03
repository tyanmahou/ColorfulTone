#pragma once
#include <Siv3D/FontAsset.hpp>

namespace ct::SharedDraw
{
	class JacketInfo
	{
	public:
		JacketInfo();
		JacketInfo& setPos(const s3d::Vec2& pos);

		const JacketInfo& drawLabel(const s3d::Texture& tex, double t = 1.0) const;
		const JacketInfo& drawLabel() const;
		const JacketInfo& drawLine() const;
		//タイトル
		const JacketInfo& drawTitle(const s3d::String& title, const s3d::Color& color = s3d::Palette::Black) const;
		// サブタイトル
		const JacketInfo& drawSub(const s3d::String& sub, const s3d::Color& color = s3d::Palette::Black) const;
		// 詳細
		const JacketInfo& drawDetail(const s3d::String& detail, const s3d::Color& color = s3d::Palette::Black) const;
		const JacketInfo& drawDetailRight(const s3d::String& detail, const s3d::Color& color = s3d::Palette::Black) const;

		const s3d::FontAsset m_font12;
		const s3d::FontAsset m_font16b;
		s3d::Vec2 m_pos;
	};
}