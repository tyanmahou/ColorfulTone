#pragma once
#include<Siv3D.hpp>
namespace util
{

	//
	struct Transformer2D:s3d::Transformer2D
	{
	
		Transformer2D(const Mat3x2& mat, bool transformMouse = false)
			: s3d::Transformer2D(
				s3d::Graphics2D::GetTransform().inverse()*mat*s3d::Graphics2D::GetTransform(),
				transformMouse)
		{}

	};

	inline void ContractionDrawbleString(const DrawableString& font, const Vec2& pos, int width,const Color& color=Palette::White, bool drawCenter = true)
	{

		const auto nameWidth = font.region().w;
		const auto getMat = [&]()
		{
			if (nameWidth > width)
			{
				const auto scale = static_cast<double>(width) / nameWidth;
				return Mat3x2::Scale(scale,1, pos);
			}
			return Mat3x2::Identity();
		};
		util::Transformer2D t2d(getMat());


		//‹È–¼
		if (drawCenter)
		{
			font.drawAt(pos, color);
		}
		else
		{
			font.draw(pos, color);
		}
	}


}