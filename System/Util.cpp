#include "Util.h"
#include<stack>
namespace
{
	std::stack<s3d::RenderTexture*> g_renderTargetStack;
}
namespace util
{
	RenderTextureUtil::RenderTextureUtil(s3d::RenderTexture& tex, const Color& color)
	{
		tex.clear(color);
		s3d::Graphics2D::SetRenderTarget(tex);
		g_renderTargetStack.push(&tex);
	}

	RenderTextureUtil::~RenderTextureUtil()
	{
		g_renderTargetStack.pop();
		auto &tex = g_renderTargetStack.empty() ? Graphics::GetSwapChainTexture() : *g_renderTargetStack.top();
		s3d::Graphics2D::SetRenderTarget(tex);
	}

	util::Transformer2D RenderTextureUtil::getTransformer2D()const
	{
		return { Graphics2D::GetTransform().inverse() };
	}

	void ContractionDrawbleString(
		const DrawableString& font,
		const Vec2& pos, 
		int width,
		const Color& color, 
		bool drawCenter
	){
		const auto nameWidth = font.region().w;
		const auto getMat = [&]()
		{
			if (nameWidth > width)
			{
				const auto scale = static_cast<double>(width) / nameWidth;
				return Mat3x2::Scale(scale, 1, pos);
			}
			return Mat3x2::Identity();
		};
		util::Transformer2D t2d(getMat());

		//曲名
		if (drawCenter)
		{
			font.drawAt(pos, color);
		}
		else
		{
			font.draw(pos, color);
		}
	}

	//マスク処理
	void StencilMask(std::function<void()> base, std::function<void()>drawFunc, StencilFunc stencilFunc, uint8 stencilValue)
	{
		uint8 cache = Graphics2D::GetStencilValue();
		Graphics2D::SetStencilState(StencilState::Replace);
		Graphics2D::SetStencilValue(stencilValue);

		base();

		Graphics2D::SetStencilState(StencilState::Test(stencilFunc));

		drawFunc();

		Graphics2D::SetStencilState(StencilState::Default);
		Graphics2D::SetStencilValue(cache);
	};
}
