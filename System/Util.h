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

	void ContractionDrawbleString(const DrawableString& font, const Vec2& pos, int width, const Color& color = Palette::White, bool drawCenter = true);

	class RenderTextureUtil
	{
	public:
		RenderTextureUtil(s3d::RenderTexture& tex, const Color& color = Palette::White);
		~RenderTextureUtil();
		util::Transformer2D getTransformer2D()const;
	};
}