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

	template<class KeyType>
	auto AccelPressed(const KeyType& key, int32 timeMillisec = 5000, int32 waitMillisec = 500)
		->decltype(key.clicked, key.pressedDuration, true)
	{
		const int32 time = timeMillisec - waitMillisec;
		const int32 interval = time<=0? 1: 10 - 9 * Min(time, (key.pressedDuration - waitMillisec)) / time;
		return key.clicked || key.pressedDuration >= waitMillisec && System::FrameCount() % interval == 0;
	}
}