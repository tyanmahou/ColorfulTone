#include <scenes/Fade/Fade.hpp>
#include <scenes/utils/Util.hpp>
#include <scenes/utils/Shaders.hpp>
#include <Siv3D.hpp>

namespace
{
	Color g_fadeColor = Palette::White;

	Image CreateImg(const Size& size = Scene::Size(), const Color& color = Palette::White)
	{
		Image img(size);
		img.fill(color);
		return std::move(img);
	}
	//微調整
	bool FadeBase(double& t)
	{
		if (t > 0.75) {
			Scene::Rect().draw(g_fadeColor);
			return false;
		}

		t *= (1 / 0.75);
		return true;
	}
}

namespace ct
{
	namespace Fade
	{
		//デフォルト
		void Default(double t)
		{
			if (!::FadeBase(t))
				return;
			Scene::Rect().draw(g_fadeColor);
		}

		//3次関数的に広がる円形マスク
		void SmoothCircle(double t)
		{
			if (!::FadeBase(t)) {
				return;
			}
			static auto func = [=](double t) {
				return ((t - 0.3f) * (t - 0.3f) * (t - 0.3f) + 0.027) / 0.37f;
				};

			{
				auto mask = Shaders::Mask().notEqual([t] { 
					Circle(Scene::Size(), Scene::Width() * func(1.0 - t)).draw(); 
				});
				Scene::Rect().draw(g_fadeColor);
			}
		}
		void FlipPage(double t, std::function<void()> drawble, bool in)
		{
			Shaders::FlipPage()
				.setTimer(t)
				.setIn(in)
				.apply(std::move(drawble))
				;
		}
		void DrawCanvas(double t, std::function<void()> drawble)
		{
			Shaders::DrawCanvas()
				.setTimer(t)
				.apply(std::move(drawble))
				;
		}
		void DrawCanvas(double t)
		{
			static Texture tex(::CreateImg(Vec2{ Scene::Width() * 1.5,Scene::Height() * 1.5 }.asPoint()));
			DrawCanvas(t, [] {tex.drawAt(Scene::Center(), g_fadeColor); });
		}
	}

	void SetFadeColor(const s3d::Color& color)
	{
		g_fadeColor = color;
	}
}
