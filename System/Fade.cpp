#include"Fade.h"
#include"Util.h"
namespace
{
	//微調整
	bool FadeBase(double& t)
	{
		if (t > 0.75)
		{
			Window::BaseClientRect().draw(ColorF(0.0));
			return false;
		}

		t *= (1 / 0.75);
		return true;
	}

	//マスク処理
	void StencilMask(std::function<void()> base, std::function<void()>drawFunc, StencilFunc stencilFunc, uint8 stencilValue = 1)
	{
		Graphics2D::SetStencilState(StencilState::Replace);
		Graphics2D::SetStencilValue(stencilValue);

		base();

		Graphics2D::SetStencilState(StencilState::Test(stencilFunc));

		drawFunc();

		Graphics2D::SetStencilState(StencilState::Default);
	};
}

namespace Fade
{
	//デフォルト
	void Default(double t)
	{
		if (!::FadeBase(t))
			return;
		Window::BaseClientRect().draw(ColorF(0.0, t));
	}

	//3次関数的に広がる円形マスク
	void SmoothCircle(double t)
	{
		if (!::FadeBase(t))
			return;

		static auto func = [=](double t)
		{
			return ((t - 0.3f)*(t - 0.3f)*(t - 0.3f) + 0.027) / 0.37f;
		};

		::StencilMask(
			[t] { Circle(Window::BaseCenter(), Window::BaseWidth() * func(1.0 - t)).draw(); },
			[] { Window::BaseClientRect().draw(ColorF(0.0, 1)); },
			StencilFunc::NotEqual
		);
	}
	void FlipPage(double t, std::function<void()> drawble)
	{
		static RenderTexture tex(Window::BaseSize());
		{
			auto render = util::RenderTextureUtil(tex);
			auto t2d = render.getTransformer2D();
			drawble();
		}
		static PixelShader ps(L"Shaders/flipPage.ps");
		static ConstantBuffer<Float4> cb;
		cb->x = t; // timer
		Graphics2D::SetConstant(ShaderStage::Pixel, 1, cb);
		Graphics2D::BeginPS(ps);
		tex.draw();
		Graphics2D::EndPS();
	}
}
