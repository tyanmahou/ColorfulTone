#include"Fade.h"
#include"Util.h"

namespace
{
	Color g_fadeColor = Palette::White;

	Image CreateImg(const Size& size = Window::BaseSize(), const Color& color = Palette::White)
	{
		Image img(size);
		img.fill(color);
		return std::move(img);
	}
	//微調整
	bool FadeBase(double& t)
	{
		if (t > 0.75)
		{
			Window::BaseClientRect().draw(g_fadeColor);
			return false;
		}

		t *= (1 / 0.75);
		return true;
	}
}

namespace Fade
{
	//デフォルト
	void Default(double t)
	{
		if (!::FadeBase(t))
			return;
		Window::BaseClientRect().draw(g_fadeColor);
	}

	//3次関数的に広がる円形マスク
	void SmoothCircle(double t)
	{
		if (!::FadeBase(t))
		{
			return;
		}
		static auto func = [=](double t)
		{
			return ((t - 0.3f)*(t - 0.3f)*(t - 0.3f) + 0.027) / 0.37f;
		};

		util::StencilMask(
			[t] { Circle(Window::BaseCenter(), Window::BaseWidth() * func(1.0 - t)).draw(); },
			[] { Window::BaseClientRect().draw(g_fadeColor); },
			StencilFunc::NotEqual
		);
	}
	struct CBufferFlipPage {
		float timer;
		int32 in;
		Float2 _unused;
	};
	void FlipPage(double t, std::function<void()> drawble, bool in)
	{
		static RenderTexture tex(Window::BaseSize());
		{
			auto render = util::RenderTextureUtil(tex);
			auto t2d = render.getTransformer2D();
			drawble();
		}
		static PixelShader ps(L"Shaders/flipPage.ps");

		static ConstantBuffer<CBufferFlipPage> cb;
		cb->timer = t; // timer
		cb->in = static_cast<int>(in);
		Graphics2D::SetConstant(ShaderStage::Pixel, 1, cb);
		Graphics2D::BeginPS(ps);
		tex.draw();
		Graphics2D::EndPS();
	}
	void DrawCanvas(double t, std::function<void()> drawble)
	{
		static PixelShader ps(L"Shaders/drawCanvas.ps");
		static Texture mask(L"Resource/Img/MusicSelect/maskDrawCanvas.png");
		static ConstantBuffer<Float4> cb;
		cb->x = t; // timer
		Graphics2D::SetTexture(ShaderStage::Pixel, 1, mask);
		Graphics2D::SetConstant(ShaderStage::Pixel, 1, cb);
		Graphics2D::BeginPS(ps);
		drawble();
		Graphics2D::EndPS();
	}
	void DrawCanvas(double t)
	{
		static Texture tex(::CreateImg({Window::BaseWidth()*1.5,Window::BaseHeight()*1.5}));
		DrawCanvas(t, [] {tex.drawAt(Window::BaseCenter(), g_fadeColor); });
	}
}

void SetFadeColor(const s3d::Color & color)
{
	g_fadeColor = color;
}
